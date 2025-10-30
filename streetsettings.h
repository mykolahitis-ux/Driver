#ifndef STREETSETTINGS_H
#define STREETSETTINGS_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QList>

// Структура для хранения диапазона номеров
struct HouseRange {
    QString streetName;
    int startNumber;
    int endNumber;
    int driverId;
    QString driverName;
};

class StreetSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit StreetSettings(QWidget *parent = nullptr);
    ~StreetSettings();

    // Метод для поиска водителя по полному адресу
    int findDriverByAddress(const QString &address, QString &driverName, QString &matchInfo);

    // Получение всех правил
    QList<HouseRange> getAllRanges() const { return houseRanges; }

private slots:
    void addNewRule();
    void deleteSelectedRule();
    void saveSettings();
    void backToMainWindow();
    void refreshTable();

private:
    void setupUI();
    void initializeDefaultData();
    void loadTableData();
    void loadSettingsFromJson();      // NEW: Load from JSON
    void saveSettingsToJson();        // NEW: Save to JSON
    int extractHouseNumber(const QString &address);

    // UI элементы
    QWidget *centralWidget;
    QLabel *titleLabel;

    // Форма добавления правила
    QLineEdit *streetInput;
    QLineEdit *startNumberInput;
    QLineEdit *endNumberInput;
    QLineEdit *driverIdInput;
    QLineEdit *driverNameInput;
    QPushButton *addButton;

    // Таблица с правилами
    QTableWidget *rulesTable;

    // Кнопки управления
    QPushButton *deleteButton;
    QPushButton *saveButton;
    QPushButton *backButton;

    // Данные
    QList<HouseRange> houseRanges;
};

#endif // STREETSETTINGS_H
