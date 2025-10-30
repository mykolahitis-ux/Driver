#include "mainwindow.h"
#include <QMessageBox>
#include "Parcelbox.h"
#include "streetsettings.h"
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    loadDriverDataFromJson();  // Changed from initializeDriverData
    setWindowTitle("Driver Search by Address");
    setFixedSize(700, 650);

    // Подключение сигналов
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchAddress);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearResults);
    connect(addressInput, &QLineEdit::returnPressed, this, &MainWindow::searchAddress);
    connect(parcelboxButton, &QPushButton::clicked, this, &MainWindow::openParcelboxWindow);
    connect(mapsButton, &QPushButton::clicked, this, &MainWindow::openMaps);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsWindow);
    connect(searchInMapsButton, &QPushButton::clicked, this, &MainWindow::searchAddressInMaps);

    parcelboxWindow = nullptr;
    settingsWindow = nullptr;
}

MainWindow::~MainWindow()
{
    if (parcelboxWindow) {
        delete parcelboxWindow;
    }
    if (settingsWindow) {
        delete settingsWindow;
    }
}

void MainWindow::loadDriverDataFromJson()
{
    QFile file("C:/Users/Босс/Desktop/хнуре/Driver/build/Desktop_Qt_6_9_2_MinGW_64_bit-Debug/drivers_data.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Failed to open drivers_data.json\n"
                              "Make sure the file exists in the application directory.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::critical(this, "Error",
                              "Invalid JSON format in drivers_data.json");
        return;
    }

    QJsonObject rootObj = doc.object();

    // Load drivers
    if (rootObj.contains("drivers") && rootObj["drivers"].isArray()) {
        QJsonArray driversArray = rootObj["drivers"].toArray();

        for (const QJsonValue &driverValue : driversArray) {
            if (!driverValue.isObject()) continue;

            QJsonObject driverObj = driverValue.toObject();

            int id = driverObj["id"].toInt();
            QString name = driverObj["name"].toString();

            QStringList streets;
            if (driverObj.contains("streets") && driverObj["streets"].isArray()) {
                QJsonArray streetsArray = driverObj["streets"].toArray();
                for (const QJsonValue &streetValue : streetsArray) {
                    streets << streetValue.toString();
                }
            }

            driverStreets[id] = streets;
            driverNames[id] = name;
        }
    }

    // Load specific addresses
    if (rootObj.contains("specificAddresses") && rootObj["specificAddresses"].isArray()) {
        QJsonArray specificArray = rootObj["specificAddresses"].toArray();

        for (const QJsonValue &addrValue : specificArray) {
            if (!addrValue.isObject()) continue;

            QJsonObject addrObj = addrValue.toObject();

            QString address = addrObj["address"].toString();
            int driverId = addrObj["driverId"].toInt();
            QString hours = addrObj["hours"].toString();

            specificAddresses[address] = driverId;
            specificHours[address] = hours;
        }
    }

    // Optional: Show success message or log
    qDebug() << "Loaded" << driverStreets.size() << "drivers and"
             << specificAddresses.size() << "specific addresses from JSON";
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

void MainWindow::openSettingsWindow()
{
    if (!settingsWindow) {
        settingsWindow = new StreetSettings(this);
    }
    settingsWindow->show();
    settingsWindow->raise();
    settingsWindow->activateWindow();
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
        "QPushButton#searchBtn { background-color: #004b93; }"
        "QPushButton#searchBtn:hover { background-color: #3a6db4; }"

        "QPushButton#clearBtn { background-color: #f44336; }"
        "QPushButton#clearBtn:hover { background-color: #da190b; }"

        "QPushButton#parcelBtn {background-color: #da7205; }"
        "QPushButton#parcelBtn:hover { background-color: #b56613; }"

        "QPushButton#mapsBtn { background-color: #0078d7; }"
        "QPushButton#mapsBtn:hover { background-color: #20B2AA; }"

        "QPushButton#settingsBtn {background-color: #9C27B0; }"
        "QPushButton#settingsBtn:hover { background-color: #7B1FA2; }"

        "QTextEdit { "
        "border: 2px solid #ddd; "
        "border-radius: 8px; "
        "padding: 10px; "
        "background-color: white; "
        "font-size: 13px; "
        "}"

        "QPushButton#searchMapsBtn { "
        "background-color: 	#FF69B4; "
        "color: white; "
        "font-weight: bold; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "} "
        "QPushButton#searchMapsBtn:hover { background-color: #FF1493; }"
        "QPushButton#settingsBtn { "
        "background-color: #9C27B0; "
        "color: white; "
        "font-weight: bold; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "} "


        "QLabel#title { "
        "font-size: 20px; "
        "font-weight: bold; "
        "color: #333; "
        "margin: 10px 0; "
        "}"
        );

    // Заголовок
    titleLabel = new QLabel("Driver Search System", centralWidget);
    titleLabel->setObjectName("title");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry(50, 40, 600, 60);

    // Поле ввода адреса
    addressInput = new QLineEdit(centralWidget);
    addressInput->setPlaceholderText("Example: Machova 15, Podnikatelska 103");
    addressInput->setGeometry(50, 130, 600, 50);

    searchButton = new QPushButton("Find Driver", centralWidget);
    searchButton->setObjectName("searchBtn");
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setGeometry(50, 200, 170, 70);

    clearButton = new QPushButton("Clear", centralWidget);
    clearButton->setObjectName("clearBtn");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setGeometry(240, 200, 170, 70);

    parcelboxButton = new QPushButton("Parcelbox Info", centralWidget);
    parcelboxButton->setObjectName("parcelBtn");
    parcelboxButton->setCursor(Qt::PointingHandCursor);
    parcelboxButton->setGeometry(430, 200, 220, 70);

    // Поле результатов
    resultText = new QTextEdit(centralWidget);
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("Driver information will be displayed here...");
    resultText->setGeometry(50, 290, 600, 280);

    // Кнопка Settings
    settingsButton = new QPushButton("⚙ Settings", centralWidget);
    settingsButton->setObjectName("settingsBtn");
    settingsButton->setCursor(Qt::PointingHandCursor);
    settingsButton->setGeometry(50, 580, 600, 50);


    // Создаем контейнер-виджет для кнопок Maps
    QWidget *mapsButtonsContainer = new QWidget(centralWidget);
    mapsButtonsContainer->setGeometry(20, 20, 660, 70);

    // Горизонтальный лайаут - выравнивает две кнопки Maps по горизонтали
    // Одна кнопка прижимается к левому краю, вторая к правому
    QHBoxLayout *mapsButtonsLayout = new QHBoxLayout(mapsButtonsContainer);
    mapsButtonsLayout->setContentsMargins(0, 0, 0, 0);
    mapsButtonsLayout->setSpacing(0);

    // Кнопка "Search in Maps" слева
    searchInMapsButton = new QPushButton("Search in Maps", mapsButtonsContainer);
    searchInMapsButton->setObjectName("searchMapsBtn");
    searchInMapsButton->setCursor(Qt::PointingHandCursor);
    searchInMapsButton->setFixedSize(140, 70);
    mapsButtonsLayout->addWidget(searchInMapsButton);

    // Растягивающийся спейсер - отодвигает кнопки к противоположным краям
    // Создает пространство между левой и правой кнопкой
    mapsButtonsLayout->addStretch();

    // Кнопка "Maps" справа
    mapsButton = new QPushButton("Maps", mapsButtonsContainer);
    mapsButton->setObjectName("mapsBtn");
    mapsButton->setCursor(Qt::PointingHandCursor);
    mapsButton->setFixedSize(120, 60);
    // Выравнивание по вертикали - центрирует кнопку по высоте относительно левой кнопки
    mapsButtonsLayout->addWidget(mapsButton, 0, Qt::AlignVCenter);
}

void MainWindow::openMaps()
{
    QUrl url("https://www.google.com/maps/search/+Kharkov+Kharkiv/?entry=wc&hl=ru");
    QDesktopServices::openUrl(url);
}

void MainWindow::searchAddressInMaps()
{
    QString address = addressInput->text().trimmed();

    if (address.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите адрес для поиска в картах");
        return;
    }

    // Автоматически добавляем "Харьков" к адресу
    QString searchQuery = address + ", Харьков";

    // Кодируем адрес для URL (заменяем пробелы и спецсимволы)
    QString encodedAddress = QUrl::toPercentEncoding(searchQuery);

    // Формируем URL для Google Maps
    QString mapsUrl = QString("https://www.google.com/maps/search/%1").arg(encodedAddress);

    // Открываем в браузере
    QDesktopServices::openUrl(QUrl(mapsUrl));
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

    // Сначала проверяем настройки диапазонов номеров домов
    if (settingsWindow) {
        QString driverName;
        QString matchInfo;
        int driverId = settingsWindow->findDriverByAddress(cleanAddress, driverName, matchInfo);

        if (driverId != -1) {
            QString result = QString(
                                 "✓ FOUND BY HOUSE NUMBER RANGE\n"
                                 "=====================================\n"
                                 "Driver ID: %1\n"
                                 "Driver Name: %2\n"
                                 "Match Details: %3\n"
                                 "=====================================\n"
                                 ).arg(driverId)
                                 .arg(driverName)
                                 .arg(matchInfo);

            resultText->setText(result);
            resultText->setStyleSheet(resultText->styleSheet() + " color: #1976D2;");
            return;
        }
    }

    // Проверка специфических адресов
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
