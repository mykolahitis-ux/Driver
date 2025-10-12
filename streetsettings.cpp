#include "streetsettings.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QRegularExpression>

StreetSettings::StreetSettings(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    initializeDefaultData();
    loadTableData();
    setWindowTitle("Street Number Range Settings");
    setFixedSize(900, 700);

    connect(addButton, &QPushButton::clicked, this, &StreetSettings::addNewRule);
    connect(deleteButton, &QPushButton::clicked, this, &StreetSettings::deleteSelectedRule);
    connect(saveButton, &QPushButton::clicked, this, &StreetSettings::saveSettings);
    connect(backButton, &QPushButton::clicked, this, &StreetSettings::backToMainWindow);
}

StreetSettings::~StreetSettings()
{
}

void StreetSettings::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QLineEdit { "
        "padding: 8px; "
        "border: 2px solid #ddd; "
        "border-radius: 6px; "
        "font-size: 13px; "
        "background-color: white; "
        "}"
        "QLineEdit:focus { border-color: #004b93; }"
        "QPushButton { "
        "padding: 10px 15px; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "color: white; "
        "margin: 5px; "
        "}"
        "QPushButton#addBtn { background-color: #4CAF50; }"
        "QPushButton#addBtn:hover { background-color: #45a049; }"
        "QPushButton#deleteBtn { background-color: #f44336; }"
        "QPushButton#deleteBtn:hover { background-color: #da190b; }"
        "QPushButton#saveBtn { background-color: #2196F3; }"
        "QPushButton#saveBtn:hover { background-color: #0b7dda; }"
        "QPushButton#backBtn { background-color: #9e9e9e; }"
        "QPushButton#backBtn:hover { background-color: #757575; }"
        "QTableWidget { "
        "border: 2px solid #ddd; "
        "border-radius: 8px; "
        "background-color: white; "
        "gridline-color: #e0e0e0; "
        "}"
        "QTableWidget::item { padding: 5px; }"
        "QTableWidget::item:selected { background-color: #bbdefb; }"
        "QHeaderView::section { "
        "background-color: #004b93; "
        "color: white; "
        "padding: 8px; "
        "border: none; "
        "font-weight: bold; "
        "}"
        "QLabel#title { "
        "font-size: 20px; "
        "font-weight: bold; "
        "color: #333; "
        "}"
        "QLabel { "
        "font-size: 12px; "
        "color: #444; "
        "font-weight: bold; "
        "}"
        "QGroupBox { "
        "border: 2px solid #ddd; "
        "border-radius: 8px; "
        "margin-top: 10px; "
        "padding-top: 15px; "
        "font-weight: bold; "
        "}"
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "left: 10px; "
        "padding: 0 5px; "
        "}"
        );

    // Заголовок
    titleLabel = new QLabel("Street House Number Range Settings", centralWidget);
    titleLabel->setObjectName("title");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry(50, 20, 800, 40);

    // Группа для добавления нового правила
    QGroupBox *addGroupBox = new QGroupBox("Add New Rule", centralWidget);
    addGroupBox->setGeometry(50, 70, 800, 140);

    QLabel *streetLabel = new QLabel("Street Name:", addGroupBox);
    streetLabel->setGeometry(20, 30, 120, 25);
    streetInput = new QLineEdit(addGroupBox);
    streetInput->setPlaceholderText("e.g., Podnikatelska");
    streetInput->setGeometry(20, 55, 150, 35);

    QLabel *startLabel = new QLabel("Start Number:", addGroupBox);
    startLabel->setGeometry(190, 30, 120, 25);
    startNumberInput = new QLineEdit(addGroupBox);
    startNumberInput->setPlaceholderText("e.g., 1");
    startNumberInput->setGeometry(190, 55, 100, 35);

    QLabel *endLabel = new QLabel("End Number:", addGroupBox);
    endLabel->setGeometry(310, 30, 120, 25);
    endNumberInput = new QLineEdit(addGroupBox);
    endNumberInput->setPlaceholderText("e.g., 50");
    endNumberInput->setGeometry(310, 55, 100, 35);

    QLabel *driverIdLabel = new QLabel("Driver ID:", addGroupBox);
    driverIdLabel->setGeometry(430, 30, 120, 25);
    driverIdInput = new QLineEdit(addGroupBox);
    driverIdInput->setPlaceholderText("e.g., 33047");
    driverIdInput->setGeometry(430, 55, 120, 35);

    QLabel *driverNameLabel = new QLabel("Driver Name:", addGroupBox);
    driverNameLabel->setGeometry(570, 30, 120, 25);
    driverNameInput = new QLineEdit(addGroupBox);
    driverNameInput->setPlaceholderText("e.g., Bochi");
    driverNameInput->setGeometry(570, 55, 120, 35);

    addButton = new QPushButton("Add Rule", addGroupBox);
    addButton->setObjectName("addBtn");
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setGeometry(250, 90, 300, 50);

    // Таблица с правилами
    rulesTable = new QTableWidget(centralWidget);
    rulesTable->setGeometry(50, 230, 800, 350);
    rulesTable->setColumnCount(5);
    rulesTable->setHorizontalHeaderLabels({"Street", "Start Number", "End Number", "Driver ID", "Driver Name"});
    rulesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    rulesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    rulesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Кнопки управления
    deleteButton = new QPushButton("Delete Selected", centralWidget);
    deleteButton->setObjectName("deleteBtn");
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setGeometry(50, 600, 190, 50);

    saveButton = new QPushButton("Save Changes", centralWidget);
    saveButton->setObjectName("saveBtn");
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setGeometry(260, 600, 190, 50);

    backButton = new QPushButton("Back to Main", centralWidget);
    backButton->setObjectName("backBtn");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setGeometry(660, 600, 190, 50);
}

void StreetSettings::initializeDefaultData()
{
    // Примеры начальных данных
    HouseRange range1;
    range1.streetName = "Podnikatelska";
    range1.startNumber = 1;
    range1.endNumber = 50;
    range1.driverId = 33047;
    range1.driverName = "Bochi";
    houseRanges.append(range1);
}

void StreetSettings::loadTableData()
{
    rulesTable->setRowCount(0);

    for (int i = 0; i < houseRanges.size(); ++i) {
        const HouseRange &range = houseRanges[i];

        int row = rulesTable->rowCount();
        rulesTable->insertRow(row);

        rulesTable->setItem(row, 0, new QTableWidgetItem(range.streetName));
        rulesTable->setItem(row, 1, new QTableWidgetItem(QString::number(range.startNumber)));
        rulesTable->setItem(row, 2, new QTableWidgetItem(QString::number(range.endNumber)));
        rulesTable->setItem(row, 3, new QTableWidgetItem(QString::number(range.driverId)));
        rulesTable->setItem(row, 4, new QTableWidgetItem(range.driverName));
    }
}

void StreetSettings::addNewRule()
{
    QString street = streetInput->text().trimmed();
    QString startStr = startNumberInput->text().trimmed();
    QString endStr = endNumberInput->text().trimmed();
    QString driverIdStr = driverIdInput->text().trimmed();
    QString driverName = driverNameInput->text().trimmed();

    if (street.isEmpty() || startStr.isEmpty() || endStr.isEmpty() ||
        driverIdStr.isEmpty() || driverName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields");
        return;
    }
//Конвертация строк в числа
    bool ok1, ok2, ok3;
    int startNum = startStr.toInt(&ok1);
    int endNum = endStr.toInt(&ok2);
    int driverId = driverIdStr.toInt(&ok3);

    if (!ok1 || !ok2 || !ok3) {
        QMessageBox::warning(this, "Error", "Numbers must be valid integers");
        return;
    }

    if (startNum > endNum) {
        QMessageBox::warning(this, "Error", "Start number must be less than or equal to end number");
        return;
    }

    HouseRange newRange;
    newRange.streetName = street;
    newRange.startNumber = startNum;
    newRange.endNumber = endNum;
    newRange.driverId = driverId;
    newRange.driverName = driverName;

    houseRanges.append(newRange);
    loadTableData();

    // Очистка полей
    streetInput->clear();
    startNumberInput->clear();
    endNumberInput->clear();
    driverIdInput->clear();
    driverNameInput->clear();

    QMessageBox::information(this, "Success", "Rule added successfully!");
}

void StreetSettings::deleteSelectedRule()
{
    int currentRow = rulesTable->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select a rule to delete");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  "Are you sure you want to delete this rule?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        houseRanges.removeAt(currentRow);
        loadTableData();
        QMessageBox::information(this, "Success", "Rule deleted successfully!");
    }
}

void StreetSettings::saveSettings()
{
    QMessageBox::information(this, "Success",
                             QString("Settings saved successfully!\nTotal rules: %1").arg(houseRanges.size()));
}

void StreetSettings::backToMainWindow()
{
    this->close();
}

int StreetSettings::extractHouseNumber(const QString &address)
{
    QRegularExpression re("\\d+");
    QRegularExpressionMatch match = re.match(address);

    if (match.hasMatch()) {
        return match.captured(0).toInt();
    }

    return -1;
}

int StreetSettings::findDriverByAddress(const QString &address, QString &driverName, QString &matchInfo)
{
    QString cleanAddress = address.toLower().simplified();

    // Извлекаем номер дома
    int houseNumber = extractHouseNumber(cleanAddress);

    if (houseNumber == -1) {
        matchInfo = "No house number found in address";
        return -1;
    }

    // Ищем подходящее правило
    for (const HouseRange &range : houseRanges) {
        QString streetLower = range.streetName.toLower();

        if (cleanAddress.contains(streetLower) || streetLower.contains(cleanAddress.split(" ")[0])) {
            if (houseNumber >= range.startNumber && houseNumber <= range.endNumber) {
                driverName = range.driverName;
                matchInfo = QString("Street: %1, House: %2 (Range: %3-%4)")
                                .arg(range.streetName)
                                .arg(houseNumber)
                                .arg(range.startNumber)
                                .arg(range.endNumber);
                return range.driverId;
            }
        }
    }

    matchInfo = QString("No rule found for house number %1").arg(houseNumber);
    return -1;
}

void StreetSettings::refreshTable()
{
    loadTableData();
}
