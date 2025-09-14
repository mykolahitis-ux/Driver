#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QMap>
#include <QStringList>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void searchAddress();
    void clearResults();

private:
    void setupUI();
    void initializeDriverData();

    // UI элементы
    QWidget *centralWidget;
    QLineEdit *addressInput;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QPushButton *parcelboxButton;
    QTextEdit *resultText;
    QLabel *titleLabel;
    QLabel *instructionLabel;

    // Данные водителей
    QMap<int, QStringList> driverStreets;
    QMap<int, QString> driverNames;

    QMap<QString, int> specificAddresses;  // ключ - полный адрес, значение - ID водителя
    QMap<QString, QString> specificHours;      // адрес -> время работы
};

#endif // MAINWINDOW_H
