QT += core widgets
QT += widgets
CONFIG += c++11

TARGET = DeliveryApp
TEMPLATE = app

SOURCES += \
    Parcelbox.cpp \
    main.cpp \
    mainwindow.cpp \
    streetsettings.cpp



HEADERS += \
    Parcelbox.h \
    mainwindow.h \
    streetsettings.h


RESOURCES += \
    images.qrc

FORMS += \
    parcelbox.ui

DISTFILES += \
    ../../../Desktop/drivers_data.json \
    drivers_data.json
