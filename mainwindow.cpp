#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    initializeDriverData();
    setWindowTitle("Driver Search by Address");
    setFixedSize(500, 400);

    // Подключение сигналов
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchAddress);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearResults);
    connect(addressInput, &QLineEdit::returnPressed, this, &MainWindow::searchAddress);
    connect(parcelboxButton, &QPushButton::clicked, this, &MainWindow::searchAddress);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Стилиы
    setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QLineEdit { "
        "padding: 10px; "
        "border: 2px solid #ddd; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "background-color: white; "
        "color: black; "
        "}"
        "QLineEdit:focus { border-color: #4CAF50; }"
        "QPushButton { "
        "padding: 10px 5px; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "color: white; "
        "margin: 10px; "
        "}"
        "QPushButton#searchBtn { background-color: #4CAF50; }"
        "QPushButton#searchBtn:hover { background-color: #45a049; }"
        "QPushButton#clearBtn { background-color: #f44336; }"
        "QPushButton#clearBtn:hover { background-color: #da190b; }"
        "QPushButton#parcelBtn { "
        "background-color: #da7205; "
        "padding: 10px 30px; "
        "min-width: 100px; "
        "}"
        "QPushButton#parcelBtn:hover { background-color: #b56613; }"
        "QTextEdit { "
        "border: 2px solid #ddd; "
        "border-radius: 8px; "
        "padding: 10px; "
        "background-color: white; "
        "font-size: 13px; "
        "}"
        "QLabel#title { "
        "font-size: 20px; "
        "font-weight: bold; "
        "color: #333; "
        "margin: 10px 0; "
        "}"
        "QLabel#instruction { "
        "font-size: 12px; "
        "color: #666; "
        "margin: 5px 0 15px 0; "
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Заголовок
    titleLabel = new QLabel("Driver Search System");
    titleLabel->setObjectName("title");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Инструкция
    instructionLabel = new QLabel("Enter street name to find responsible driver");
    instructionLabel->setObjectName("instruction");
    instructionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(instructionLabel);

    // Поле ввода адреса
    addressInput = new QLineEdit();
    addressInput->setPlaceholderText("Example: Machova, Majerova, Skupova");
    mainLayout->addWidget(addressInput);

    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    searchButton = new QPushButton("Find Driver");
    searchButton->setObjectName("searchBtn");
    searchButton->setCursor(Qt::PointingHandCursor);

    clearButton = new QPushButton("Clear");
    clearButton->setObjectName("clearBtn");
    clearButton->setCursor(Qt::PointingHandCursor);

    parcelboxButton = new QPushButton("Parcelbox Info");
    parcelboxButton->setObjectName("parcelBtn");
    parcelboxButton->setCursor(Qt::PointingHandCursor);

    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(parcelboxButton);
    mainLayout->addLayout(buttonLayout);

    // Поле результатов
    resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("Driver information will be displayed here...");
    mainLayout->addWidget(resultText);
}

void MainWindow::initializeDriverData()
{

    driverStreets[53455] = QStringList() << "emila skody" << "skupova" << "tylova";
    driverNames[53455] = "Tomasek";

    driverStreets[33453] = QStringList() << "machova" << "cermakova" << "politickych veznu" << "mirova";
    driverNames[33453] = "Sheba";

    driverStreets[33047] = QStringList() << "brnenska" << "podnikatelska" << "univerzitni";
    driverNames[33047] = "Bochi";

    driverStreets[33051] = QStringList() << "hrbitovni" << "zabelska" << "cvokarska";
    driverNames[33051] = "Debil";

    specificAddresses["hrbitovni 64"] = 33047;
    specificHours["hrbitovni 64"] = "8:00 - 20:00";

    specificAddresses["podnikatelska 1"] = 33047;
    specificHours["podnikatelska 1"] = "10:00 - 13:00";
}

void MainWindow::searchAddress()
{
    QString address = addressInput->text().trimmed();  // убирает лишние пробелы по краям

    if (address.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an address to search");
        return;
    }

    // Приведение к нижнему регистру для поиска (только буквы и цифры (символы)
    QString searchAddress = address.toLower();

    // Убираем все символы и оставляем только буквы, цифры и пробелы
    QString cleanAddress;
    for (QChar c : searchAddress) {
        if (c.isLetterOrNumber() || c.isSpace()) {
            cleanAddress += c;
        }
    }
    cleanAddress = cleanAddress.simplified(); // убирает лишние пробелы

    // НОВОЕ: Сначала проверяем конкретные адреса
    if (specificAddresses.contains(cleanAddress)) {
        int specificDriverId = specificAddresses[cleanAddress];
        QString workHours = specificHours[cleanAddress];
        QString result = QString(
                             "SPECIFIC ADDRESS DELIVERY\n"
                             "Package will be delivered by driver ID: %1\n"
                             "Driver name: %2\n"
                             "Working hours: %3\n"
                             ).arg(specificDriverId)
                             .arg(driverNames[specificDriverId])
                             .arg(workHours);

        resultText->setText(result);
        resultText->setStyleSheet(resultText->styleSheet() + " color: #9f07e0;");  // фиолетовый фиол цвет для специальных адресов
        return;
    }


    // Поиск водителя
    int foundDriverId = -1;
    QString foundStreet;

    for (auto it = driverStreets.begin(); it != driverStreets.end(); ++it) {
        int driverId = it.key();
        QStringList streets = it.value();

        for (const QString &street : streets) {
            // Улучшенный поиск: ищем частичные совпадения в обе стороны
            if (cleanAddress.contains(street) || street.contains(cleanAddress)) {
                foundDriverId = driverId;
                foundStreet = street;
                break;
            }
        }
        if (foundDriverId != -1) {
            break;
        }
    }

    QString result;

    // Отображение результата
    if (foundDriverId != -1) {
        result = QString(
                     "Package will be delivered by driver ID: %1\n"
                     "Driver name: %2\n"
                     "Assigned streets: %3\n"
                     "Found match: %4\n"
                     ).arg(foundDriverId)
                     .arg(driverNames[foundDriverId])
                     .arg(driverStreets[foundDriverId].join(", "))
                     .arg(foundStreet);

        resultText->setStyleSheet(resultText->styleSheet() + " color: #2e7d32;");
    } else {
        result = QString(
                     "DRIVER NOT FOUND\n"
                     "Address: %1\n"
                     "Please check the spelling or try a different street name.\n"
                     ).arg(address);

        resultText->setStyleSheet("border: 2px solid #ddd; border-radius: 8px; padding: 10px; background-color: white; color: #c62828; font-size: 13px;");
    }

    resultText->setText(result);
}

void MainWindow::clearResults()
{
    addressInput->clear();
    resultText->clear();
    addressInput->setFocus();
}
