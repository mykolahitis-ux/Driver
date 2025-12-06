#ifndef PARCELBOX_H
#define PARCELBOX_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QMap>

struct ParcelboxInfo {
    int driverId;
    QString street;
    QString boxNumber;
    QString workingHours;
    QString driverName;
};

class Parcelbox : public QMainWindow
{
    Q_OBJECT

public:
    Parcelbox(QWidget *parent = nullptr);
    ~Parcelbox();

    // Публичный метод для загрузки данных
    void loadParcelboxDataFromJson();

private slots:
    void searchByComboBox();
    void searchByAddress();
    void clearResults();
    void backToMainWindow();

private:
    void setupUI();
    void updateComboBoxItems();

    // UI элементы
    QWidget *centralWidget;
    QLabel *titleLabel;
    QLabel *comboLabel;
    QLabel *addressLabel;
    QComboBox *parcelboxComboBox;
    QLineEdit *addressInput;
    QPushButton *searchComboButton;
    QPushButton *searchAddressButton;
    QPushButton *clearButton;
    QPushButton *backButton;
    QTextEdit *resultText;

    // Данные
    QMap<QString, ParcelboxInfo> parcelboxData;
    QStringList parcelboxList;
};

#endif // PARCELBOX_H
