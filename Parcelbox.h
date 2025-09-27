#ifndef PARCELBOX_H
#define PARCELBOX_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QMap>
#include <QStringList>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

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

private slots:
    void searchByComboBox();
    void searchByAddress();
    void clearResults();
    void backToMainWindow();

private:
    void setupUI();
    void initializeParcelboxData();
    void updateComboBoxItems();

    // UI элементы
    QWidget *centralWidget;
    QComboBox *parcelboxComboBox;
    QLineEdit *addressInput;
    QPushButton *searchComboButton;
    QPushButton *searchAddressButton;
    QPushButton *clearButton;
    QPushButton *backButton;
    QTextEdit *resultText;
    QLabel *titleLabel;
    QLabel *instructionLabel;
    QLabel *comboLabel;
    QLabel *addressLabel;

    // Данные парселбоксов
    QMap<QString, ParcelboxInfo> parcelboxData; // ключ - полное название парселбокса
    QStringList parcelboxList; // список всех парселбоксов для комбо бокса
};

#endif // PARCELBOX_H
