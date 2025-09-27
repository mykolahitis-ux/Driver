QT += core widgets
QT += widgets
CONFIG += c++11

TARGET = DeliveryApp
TEMPLATE = app

SOURCES += \
    Parcelbox.cpp \
    main.cpp \
    mainwindow.cpp \


HEADERS += \
    Parcelbox.h \
    mainwindow.h

RESOURCES += \
    images.qrc

FORMS += \
    parcelbox.ui
