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

INCLUDEPATH += . players network textgui

SOURCES += main.cpp\
    server.cpp \
    cardpile.cpp \
    team.cpp \
    cardlist.cpp \
    textgui/mainwindow.cpp \
    textgui/textgui.cpp \
    network/agserver.cpp \
    network/chatserver.cpp \
    players/player.cpp \
    players/berserker.cpp \
    players/archer.cpp \
    players/sealer.cpp \
    players/assassin.cpp \
    players/blademaster.cpp \
    players/saintess.cpp \
    players/magicsword.cpp \
    players/holyspear.cpp \
    players/magister.cpp \
    players/elementalist.cpp \
    players/adventurer.cpp \
    players/angel.cpp \
    players/necromancer.cpp \
    players/arbiter.cpp \
    players/prayer.cpp \
    players/oracle.cpp \
    players/paladin.cpp


HEADERS  += server.h \
    team.h \
    cardpile.h \
    termination.h \
    cardlist.h \
    textgui/mainwindow.h \
    textgui/textgui.h \
    network/agserver.h \
    network/chatserver.h \
    players/player.h \
    players/berserker.h \
    players/archer.h \
    players/sealer.h \
    players/assassin.h \
    players/blademaster.h \
    players/saintess.h \
    players/magicsword.h \
    players/holyspear.h \
    players/magister.h \
    players/elementalist.h \
    players/adventurer.h \
    players/angel.h \
    players/necromancer.h \
    players/arbiter.h \
    players/prayer.h \
    players/oracle.h \
    players/paladin.h

FORMS    +=

OTHER_FILES += \
    RunExample.txt \
    changefoethechapter.txt \
    kindOfMessage.txt \
    characterNum.txt \
    InformationNew.txt
