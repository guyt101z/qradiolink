#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T13:18:22
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = qradiolink
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    speech.cpp \
    audiointerface.cpp \
    dtmfdecoder.cpp \
    telnetclient.cpp \
    telnetserver.cpp \
    serverwrapper.cpp \
    audioclient.cpp \
    dtmfcommand.cpp \
    databaseapi.cpp

HEADERS += speech.h \
    audiointerface.h \
    dtmfdecoder.h \
    telnetclient.h \
    telnetserver.h \
    serverwrapper.h \
    audioclient.h \
    dtmfcommand.h \
    databaseapi.h



unix:!symbian: LIBS += -lespeak -lpulse-simple -L$$PWD/iaxclient/build -liaxclient_lib
DEPENDPATH += $$PWD/iaxclient/build


