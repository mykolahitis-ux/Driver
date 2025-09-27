#include "mainwindow.h"
#include <QMessageBox>
#include "Parcelbox.h"

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
    connect(parcelboxButton, &QPushButton::clicked, this, &MainWindow::openParcelboxWindow);
    parcelboxWindow = nullptr;
}

MainWindow::~MainWindow()
{
    if (parcelboxWindow) {
        delete parcelboxWindow;
    }
}

void MainWindow::openParcelboxWindow()
{
    if (!parcelboxWindow) {
        parcelboxWindow = new Parcelbox(this);
    }
    parcelboxWindow->show();
    parcelboxWindow->raise();
    parcelboxWindow->activateWindow();
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Стили
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
        "QLineEdit:focus { border-color: #004b93; }"
        "QPushButton { "
        "padding: 10px 5px; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "color: white; "
        "margin: 10px; "
        "}"
        "QPushButton#searchBtn { background-color: #004b93; }" // found adres btn
        "QPushButton#searchBtn:hover { background-color: #3a6db4; }" // когда навели курсор
        "QPushButton#clearBtn { background-color: #f44336; }" // очистить поле
        "QPushButton#clearBtn:hover { background-color: #da190b; }"// когда навели курсор
        "QPushButton#parcelBtn { "
        "background-color: #da7205; " // parcelobx цвет
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

    // Заголовок - позиция и размер
    titleLabel = new QLabel("Driver Search System", centralWidget);
    titleLabel->setObjectName("title");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry(30, 30, 440, 40);

    // Поле ввода адреса
    addressInput = new QLineEdit(centralWidget);
    addressInput->setPlaceholderText("Example: Machova, Majerova, Skupova");
    addressInput->setGeometry(30, 105, 440, 40);

    // Кнопки в ряд
    searchButton = new QPushButton("Find Driver", centralWidget);
    searchButton->setObjectName("searchBtn");
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setGeometry(30, 160, 130, 60);

    clearButton = new QPushButton("Clear", centralWidget);
    clearButton->setObjectName("clearBtn");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setGeometry(175, 160, 130, 60);

    parcelboxButton = new QPushButton("Parcelbox Info", centralWidget);
    parcelboxButton->setObjectName("parcelBtn");
    parcelboxButton->setCursor(Qt::PointingHandCursor);
    parcelboxButton->setGeometry(320, 160, 150, 60);

    // Поле результатов
    resultText = new QTextEdit(centralWidget);
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("Driver information will be displayed here...");
    resultText->setGeometry(30, 215, 440, 155);
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
    QString address = addressInput->text().trimmed();

    if (address.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an address to search");
        return;
    }

    QString searchAddress = address.toLower();

    QString cleanAddress;
    for (QChar c : searchAddress) {
        if (c.isLetterOrNumber() || c.isSpace()) {
            cleanAddress += c;
        }
    }
    cleanAddress = cleanAddress.simplified();

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
        resultText->setStyleSheet(resultText->styleSheet() + " color: #9f07e0;");
        return;
    }

    int foundDriverId = -1;
    QString foundStreet;

    for (auto it = driverStreets.begin(); it != driverStreets.end(); ++it) {
        int driverId = it.key();
        QStringList streets = it.value();

        for (const QString &street : streets) {
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
