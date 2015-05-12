#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T15:48:18
#
#-------------------------------------------------

QT       += gui network widgets testlib multimedia

TARGET = tst_yasctestsuitetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_yasctestsuitetest.cpp \
    ../server/serverwindow.cpp \
    ../server/yascserver.cpp \
    ../server/simplecrypt.cpp \
    ../client/yascclient.cpp \
    ../client/initialwindow.cpp \
    ../client/mainwindow.cpp

HEADERS  += ../server/serverwindow.h \
    ../server/yascserver.h \
    ../server/simplecrypt.h \
    ../client/yascclient.h \
    ../client/initialwindow.h \
    ../client/mainwindow.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

FORMS    += ../server/serverwindow.ui \
    ../client/initialwindow.ui \
    ../client/mainwindow.ui

CONFIG += c++11
