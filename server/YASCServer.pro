#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T16:42:41
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = YASCServer
TEMPLATE = app


SOURCES += main.cpp\
    yascserver.cpp \
    simplecrypt.cpp \
    serverwindow.cpp

HEADERS  += \
    yascserver.h \
    simplecrypt.h \
    serverwindow.h

FORMS    += \
    serverwindow.ui

CONFIG += c++11
