#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
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
    void openSettingsWindow();
    void searchAddressInMaps();  // НОВАЯ ФУНКЦИЯ

private:
    void setupUI();
    void initializeDriverData();

    // UI элементы
    QWidget *centralWidget;
    QLabel *titleLabel;
    QLineEdit *addressInput;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QPushButton *parcelboxButton;
    QPushButton *mapsButton;
    QPushButton *searchInMapsButton;  // НОВАЯ КНОПКА
    QPushButton *settingsButton;
    QTextEdit *resultText;

    // Данные водителей
    QMap<int, QStringList> driverStreets;
    QMap<int, QString> driverNames;
    QMap<QString, int> specificAddresses;
    QMap<QString, QString> specificHours;

    // Окна
    Parcelbox *parcelboxWindow;
    StreetSettings *settingsWindow;
};

#endif // MAINWINDOW_H
