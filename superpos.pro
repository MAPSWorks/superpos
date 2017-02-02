#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T12:41:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = superpos
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    drawer.cpp \
    parser.cpp \
    locator.cpp \
    target.cpp

HEADERS  += widget.h \
    drawer.h \
    parser.h \
    rls_header.h \
    locator.h \
    target.h

FORMS    += widget.ui
