#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T16:44:39
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YASCclient
TEMPLATE = app


SOURCES += main.cpp\
    yascclient.cpp \
    initialwindow.cpp \
    mainwindow.cpp \
    simplecrypt.cpp

HEADERS  += \
    yascclient.h \
    initialwindow.h \
    mainwindow.h \
    simplecrypt.h

CONFIG += c++11

FORMS += \
    initialwindow.ui \
    mainwindow.ui
