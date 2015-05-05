#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T16:42:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YASCServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    yascserver.cpp \
    simplecrypt.cpp

HEADERS  += mainwindow.h \
    yascserver.h \
    simplecrypt.h

FORMS    += mainwindow.ui

CONFIG += c++11
