#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T12:41:25
#
#-------------------------------------------------

QT += core gui widgets network

CONFIG += c++11

TARGET = superpos
TEMPLATE = app

INCLUDEPATH += $(QMAPCONTROL_INCL) $(GEOGRAPHIC_INCL)
DEPENDPATH += $${INCLUDEPATH}

LIBS += -L$(GEOGRAPHIC_LIB) -L$(QMAPCONTROL_LIB) -lGeographic -lqmapcontrol

SOURCES += main.cpp\
    widget.cpp \
    parser.cpp \
    locator.cpp \
    target.cpp \
    mapviewer.cpp \
    dialog.cpp \
    loc_widget.cpp \
    motion.cpp

HEADERS  += widget.h \
    parser.h \
    rls_header.h \
    locator.h \
    target.h \
    mapviewer.h \
    dialog.h \
    loc_widget.h \
    motion.h

FORMS    += widget.ui

DISTFILES +=
