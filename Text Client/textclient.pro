#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T17:52:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = textclient6
TEMPLATE = app
QMAKE_LFLAGS += -lboost_system-mt

SOURCES += main.cpp\
        mainwindow.cpp \
    AGClient.cpp

HEADERS  += mainwindow.h \
    AGClient.h

FORMS    += mainwindow.ui
