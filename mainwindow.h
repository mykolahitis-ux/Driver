#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QMap>
#include <QStringList>

class Parcelbox;
class StreetSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void searchAddress();
    void clearResults();
    void openParcelboxWindow();
    void openMaps();
    void searchAddressInMaps();
    void openSettingsWindow();

private:
    void setupUI();
    void loadDriverDataFromJson();
    void saveDriverDataToJson();  // Новый метод для сохранения
    QString getDataFilePath(const QString &fileName);  // Новый метод для путей

    // UI элементы
    QWidget *centralWidget;
    QLabel *titleLabel;
    QLineEdit *addressInput;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QPushButton *parcelboxButton;
    QPushButton *mapsButton;
    QPushButton *settingsButton;
    QPushButton *searchInMapsButton;
    QTextEdit *resultText;

    // Данные водителей
    QMap<int, QStringList> driverStreets;
    QMap<int, QString> driverNames;
    QMap<QString, int> specificAddresses;
    QMap<QString, QString> specificHours;

    // Дочерние окна
    Parcelbox *parcelboxWindow;
    StreetSettings *settingsWindow;
};

#endif // MAINWINDOW_H
