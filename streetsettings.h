#ifndef STREETSETTINGS_H
#define STREETSETTINGS_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QList>

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
    StreetSettings(QWidget *parent = nullptr);
    ~StreetSettings();

    // Публичные методы для работы с данными
    void loadSettingsFromJson();
    int findDriverByAddress(const QString &address, QString &driverName, QString &matchInfo);
    void refreshTable();

private slots:
    void addNewRule();
    void deleteSelectedRule();
    void saveSettings();
    void backToMainWindow();

private:
    void setupUI();
    void saveSettingsToJson();
    void initializeDefaultData();
    void loadTableData();
    int extractHouseNumber(const QString &address);

    // UI элементы
    QWidget *centralWidget;
    QLabel *titleLabel;
    QLineEdit *streetInput;
    QLineEdit *startNumberInput;
    QLineEdit *endNumberInput;
    QLineEdit *driverIdInput;
    QLineEdit *driverNameInput;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *saveButton;
    QPushButton *backButton;
    QTableWidget *rulesTable;

    // Данные
    QList<HouseRange> houseRanges;
};

#endif // STREETSETTINGS_H
