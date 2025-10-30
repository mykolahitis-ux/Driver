#include "parcelbox.h"
#include <QMessageBox>
#include <QGroupBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Parcelbox::Parcelbox(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    loadParcelboxDataFromJson();
    updateComboBoxItems();
    setWindowTitle("Parcelbox Information System");
    setFixedSize(600, 500);

    // Подключение сигналов
    connect(searchComboButton, &QPushButton::clicked, this, &Parcelbox::searchByComboBox);
    connect(searchAddressButton, &QPushButton::clicked, this, &Parcelbox::searchByAddress);
    connect(clearButton, &QPushButton::clicked, this, &Parcelbox::clearResults);
    connect(backButton, &QPushButton::clicked, this, &Parcelbox::backToMainWindow);
    connect(addressInput, &QLineEdit::returnPressed, this, &Parcelbox::searchByAddress);
}

Parcelbox::~Parcelbox()
{
}

void Parcelbox::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Стили с синим цветом для Find Parcelbox
    setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QLineEdit, QComboBox { "
        "padding: 10px; "
        "border: 2px solid #ddd; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "background-color: white; "
        "color: black; "
        "}"
        "QLineEdit:focus, QComboBox:focus { border-color: #004b93; }"
        "QPushButton { "
        "padding: 15px 10px; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 16px; "
        "font-weight: bold; "
        "color: white; "
        "margin: 5px; "
        "}"
        "QPushButton#searchComboBtn { background-color: #ff9800; }"
        "QPushButton#searchComboBtn:hover { background-color: #f57c00; }"
        "QPushButton#searchAddressBtn { background-color: #004b93; }"
        "QPushButton#searchAddressBtn:hover { background-color: #3a6db4; }"
        "QPushButton#clearBtn { background-color: #f44336; }"
        "QPushButton#clearBtn:hover { background-color: #da190b; }"
        "QPushButton#backBtn { background-color: #9e9e9e; }"
        "QPushButton#backBtn:hover { background-color: #757575; }"
        );

    // Заголовок
    titleLabel = new QLabel("Parcelbox Information System", centralWidget);
    titleLabel->setObjectName("title");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry(30, 20, 540, 50);

    // ЛЕВАЯ СЕКЦИЯ - Комбо бокс
    comboLabel = new QLabel("Select Parcelbox:", centralWidget);
    comboLabel->setGeometry(30, 105, 250, 25);

    parcelboxComboBox = new QComboBox(centralWidget);
    parcelboxComboBox->setEditable(false);
    parcelboxComboBox->setGeometry(30, 135, 250, 40);

    searchComboButton = new QPushButton("Get Parcelbox Info", centralWidget);
    searchComboButton->setObjectName("searchComboBtn");
    searchComboButton->setCursor(Qt::PointingHandCursor);
    searchComboButton->setGeometry(30, 185, 250, 60);

    // ПРАВАЯ СЕКЦИЯ - Поиск по адресу
    addressLabel = new QLabel("Enter street name:", centralWidget);
    addressLabel->setGeometry(320, 105, 250, 25);

    addressInput = new QLineEdit(centralWidget);
    addressInput->setPlaceholderText("Example: Machova, Brnenska, Skupova");
    addressInput->setGeometry(320, 135, 250, 40);

    searchAddressButton = new QPushButton("Find Parcelbox", centralWidget);
    searchAddressButton->setObjectName("searchAddressBtn");
    searchAddressButton->setCursor(Qt::PointingHandCursor);
    searchAddressButton->setGeometry(320, 185, 250, 60);

    // Кнопки управления
    clearButton = new QPushButton("Clear", centralWidget);
    clearButton->setObjectName("clearBtn");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setGeometry(30, 265, 250, 60);

    backButton = new QPushButton("Back to Main", centralWidget);
    backButton->setObjectName("backBtn");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setGeometry(320, 265, 250, 60);

    // Поле результатов
    resultText = new QTextEdit(centralWidget);
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("Parcelbox information will be displayed here...");
    resultText->setGeometry(30, 330, 540, 140);
}

void Parcelbox::loadParcelboxDataFromJson()
{
    QFile file("C:/Users/Босс/Desktop/хнуре/Driver/build/Desktop_Qt_6_9_2_MinGW_64_bit-Debug/parcelbox_data.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Failed to open parcelbox_data.json\n"
                              "Make sure the file exists in the application directory.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::critical(this, "Error",
                              "Invalid JSON format in parcelbox_data.json");
        return;
    }

    QJsonObject rootObj = doc.object();

    // Load parcelboxes
    if (rootObj.contains("parcelboxes") && rootObj["parcelboxes"].isArray()) {
        QJsonArray parcelboxArray = rootObj["parcelboxes"].toArray();

        for (const QJsonValue &boxValue : parcelboxArray) {
            if (!boxValue.isObject()) continue;

            QJsonObject boxObj = boxValue.toObject();

            ParcelboxInfo info;
            info.driverId = boxObj["driverId"].toInt();
            info.street = boxObj["street"].toString();
            info.boxNumber = boxObj["boxNumber"].toString();
            info.workingHours = boxObj["workingHours"].toString();
            info.driverName = boxObj["driverName"].toString();

            // Создаем ключ в формате "Street - BoxNumber"
            QString key = QString("%1 - %2").arg(info.street).arg(info.boxNumber);
            parcelboxData[key] = info;
        }
    }

    // Optional: Show success message or log
    qDebug() << "Loaded" << parcelboxData.size() << "parcelboxes from JSON";
}

void Parcelbox::updateComboBoxItems()
{
    parcelboxList = parcelboxData.keys();
    parcelboxList.sort();
    parcelboxComboBox->addItems(parcelboxList);
}

void Parcelbox::searchByComboBox()
{
    QString selectedParcelbox = parcelboxComboBox->currentText();

    if (selectedParcelbox.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a parcelbox from the list");
        return;
    }

    if (parcelboxData.contains(selectedParcelbox)) {
        ParcelboxInfo info = parcelboxData[selectedParcelbox];
        QString result = QString(
                             "PARCELBOX INFORMATION\n"
                             "=====================================\n"
                             "Parcelbox: %1\n"
                             "Street: %2\n"
                             "Box Number: %3\n"
                             "Working Hours: %4\n"
                             "=====================================\n"
                             "Responsible Driver ID: %5\n"
                             "Driver Name: %6\n"
                             ).arg(selectedParcelbox)
                             .arg(info.street)
                             .arg(info.boxNumber)
                             .arg(info.workingHours)
                             .arg(info.driverId)
                             .arg(info.driverName);

        resultText->setText(result);
        resultText->setStyleSheet(resultText->styleSheet() + " color: #ff9800;");
    }
}

void Parcelbox::searchByAddress()
{
    QString address = addressInput->text().trimmed();

    if (address.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an address to search");
        return;
    }

    // Приведение к нижнему регистру для поиска
    QString searchAddress = address.toLower();

    // Убираем все символы кроме букв, цифр и пробелов
    QString cleanAddress;
    for (QChar c : searchAddress) {
        if (c.isLetterOrNumber() || c.isSpace()) {
            cleanAddress += c;
        }
    }
    cleanAddress = cleanAddress.simplified();

    // Поиск парселбокса
    QStringList foundParcelboxes;

    for (auto it = parcelboxData.begin(); it != parcelboxData.end(); ++it) {
        QString parcelboxName = it.key();
        ParcelboxInfo info = it.value();
        QString streetName = info.street.toLower();

        // Ищем частичные совпадения
        if (cleanAddress.contains(streetName) || streetName.contains(cleanAddress)) {
            foundParcelboxes.append(parcelboxName);
        }
    }

    QString result;

    if (!foundParcelboxes.isEmpty()) {
        result = "FOUND PARCELBOXES\n=====================================\n";

        for (const QString &parcelboxName : foundParcelboxes) {
            ParcelboxInfo info = parcelboxData[parcelboxName];
            result += QString(
                          "Parcelbox: %1\n"
                          "Street: %2\n"
                          "Box Number: %3\n"
                          "Working Hours: %4\n"
                          "Driver ID: %5 (%6)\n"
                          "-------------------------------------\n"
                          ).arg(parcelboxName)
                          .arg(info.street)
                          .arg(info.boxNumber)
                          .arg(info.workingHours)
                          .arg(info.driverId)
                          .arg(info.driverName);
        }

        resultText->setStyleSheet(resultText->styleSheet() + " color: #2e7d32;");
    } else {
        result = QString(
                     "NO PARCELBOXES FOUND\n"
                     "=====================================\n"
                     "Address: %1\n"
                     "Please check the spelling or try a different street name.\n"
                     ).arg(address);

        resultText->setStyleSheet("border: 2px solid #ddd; border-radius: 8px; padding: 10px; background-color: white; color: #c62828; font-size: 13px;");
    }

    resultText->setText(result);
}

void Parcelbox::clearResults()
{
    addressInput->clear();
    resultText->clear();
    parcelboxComboBox->setCurrentIndex(0);
    addressInput->setFocus();
}

void Parcelbox::backToMainWindow()
{
    this->close();
}
