#-------------------------------------------------
#
# Project created by QtCreator 2021-01-23T19:51:10
#
#-------------------------------------------------

QT       += core gui serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GEN24_ModbusTCP_storagetest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ../GEN24_ModbusTCP_library/gen24_data_modbus_helperfuncs.cpp \
    ../GEN24_ModbusTCP_library/gen24_data_modbus_tcp.cpp \
        main.cpp \
        mainwindow.cpp \
    console.cpp

HEADERS += \
    ../GEN24_ModbusTCP_library/gen24_data_modbus_definesstructsenums.h \
    ../GEN24_ModbusTCP_library/gen24_data_modbus_helperfuncs.h \
    ../GEN24_ModbusTCP_library/gen24_data_modbus_tcp.h \
        mainwindow.h \
    console.h \
    global.h

FORMS += \
        mainwindow.ui

QMAKE_LFLAGS += -no-pie

