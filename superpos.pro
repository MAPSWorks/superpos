#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T12:41:25
#
#-------------------------------------------------
include(QMapControl.pri)

QT += network
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = superpos
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    parser.cpp \
    locator.cpp \
    target.cpp \
    mapviewer.cpp

HEADERS  += widget.h \
    parser.h \
    rls_header.h \
    locator.h \
    target.h \
    mapviewer.h

LIBS += -lGeographic

FORMS    += widget.ui

DISTFILES += \
    QMapControl.pri
