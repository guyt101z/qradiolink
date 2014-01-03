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
    dtmfcommand.cpp \
    databaseapi.cpp \
    controller.cpp \
    station.cpp \
    server.cpp \
    ext/CryptState.cpp \
    ext/Timer.cpp \
    ext/Mumble.pb.cc \
    ext/QRadioLink.pb.cc \
    audioencoder.cpp \
    mumbleclient.cpp \
    sslclient.cpp \
    ext/utils.cpp


HEADERS += speech.h \
    audiointerface.h \
    dtmfdecoder.h \
    telnetclient.h \
    telnetserver.h \
    serverwrapper.h \
    dtmfcommand.h \
    databaseapi.h \
    controller.h \
    station.h \
    server.h \
    config_defines.h \
    ext/CryptState.h \
    ext/Timer.h \
    ext/Mumble.pb.h \
    ext/murmur_pch.h \
    ext/PacketDataStream.h \
    ext/QRadioLink.pb.h \
    audioencoder.h \
    mumbleclient.h \
    sslclient.h \
    ext/utils.h




unix:!symbian: LIBS += -lssl -lprotobuf -lopus -lestools -lFestival -lpulse-simple
unix:!symbian: LIBS += -L$$PWD/codec2 -lcodec2
unix:!symbian: LIBS += -L$$PWD/iaxclient/build -liaxclient_lib
DEPENDPATH += $$PWD/iaxclient/build
INCLUDEPATH += /usr/include/speech_tools


