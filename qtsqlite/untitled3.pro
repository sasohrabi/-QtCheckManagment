#-------------------------------------------------
#
# Project created by QtCreator 2013-11-01T20:08:42
#
#-------------------------------------------------

QT       += core gui sql declarative widgets xml script

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

DEFINES += NO_BARCODE
include(../bcksqlite/QtRPT/QtRPT.pri)

TARGET = untitled3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    shamsi/qdateconvertor.cpp \


HEADERS  += mainwindow.h \
    shamsi/qdateconvertor.h

FORMS    += mainwindow.ui

CONFIG += static
