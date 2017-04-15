#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T11:31:04
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = casomiera
TEMPLATE = app
RESOURCES = startdialog.qrc

SOURCES += main.cpp\
        startdialog.cpp \
    communicator.cpp

HEADERS  += startdialog.h \
    communicator.h

FORMS    += startdialog.ui

mac {
    LIBS += -framework AppKit -framework IOKit
    ICON = icons/icon.icns
}

win32 {
    RC_ICONS = icons/icon.ico
}
