#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T11:31:04
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = casomiera
TEMPLATE = app


SOURCES += main.cpp\
        startdialog.cpp

HEADERS  += startdialog.h

FORMS    += startdialog.ui

macx {
  ImageFiles.files = images
  ImageFiles.path = Contents/Resources
  QMAKE_BUNDLE_DATA += ImageFiles
}



