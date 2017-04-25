#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T11:31:04
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StopWatch
TEMPLATE = app
RESOURCES = startdialog.qrc

SOURCES += main.cpp\
        startdialog.cpp \
    communicator.cpp \
    rest_client.cpp

HEADERS  += startdialog.h \
    communicator.h \
    rest_client.h

FORMS    += startdialog.ui

unix {
    QMAKE_POST_LINK  = 'cp ../scripts/install_unity.sh .'
}

mac {
    LIBS += -framework AppKit -framework IOKit
    ICON = icons/icon.icns
}

win32 {
    RC_ICONS = icons/icon.ico
}
