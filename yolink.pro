#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T13:18:22
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = yolink
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
        DtmfGenerator.cpp \
        DtmfDetector.cpp \
         decoder.cpp \
    speech.cpp \
    dtmf.cpp \
    audiointerface.cpp \
    dtmfdecoder.cpp

HEADERS += DtmfDetector.hpp \
        DtmfGenerator.hpp \
        types_cpp.hpp \
    speech.h \
    dtmf.h \
    audiointerface.h \
    dtmfdecoder.h


unix:!symbian: LIBS += -lespeak -lpulse-simple
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD
