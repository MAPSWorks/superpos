#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T12:41:25
#
#-------------------------------------------------

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
    mapviewer.cpp \
    dialog.cpp \
    loc_widget.cpp

HEADERS  += widget.h \
    parser.h \
    rls_header.h \
    locator.h \
    target.h \
    mapviewer.h \
    dialog.h \
    loc_widget.h

#INCLUDEPATH += $${GEOGRAPHIC_INCL}:$${QMAPCONTROL_INCL}
#LIBS += -L$${GEOGRAPHIC_LIB}:$${QMAPCONTROL_LIB} -lGeographic -lqmapcontrol
LIBS += -lqmapcontrol

FORMS    += widget.ui

DISTFILES +=
