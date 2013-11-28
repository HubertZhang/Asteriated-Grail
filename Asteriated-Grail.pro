#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T14:45:25
#
#-------------------------------------------------

QT       += core gui
QT		 += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Asteriated-Grail
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    player.cpp \
    card.cpp \
    cardList.cpp \
    cardpile.cpp \
    team.cpp \
    blademaster.cpp \
    textgui.cpp \
    agserver.cpp

HEADERS  += mainwindow.h \
    server.h \
    player.h \
    card.h \
    cardList.h \
    team.h \
    cardpile.h \
    termination.h \
    blademaster.h \
    textgui.h \
    agserver.h

FORMS    +=

OTHER_FILES += \
    RunExample.txt \
    changefoethechapter.txt \
    kindOfMessage.txt \
    characterNum.txt
