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
    motion.cpp \
    params_groupbox.cpp \
    locators_ctrl.cpp

HEADERS  += widget.h \
    parser.h \
    rls_header.h \
    locator.h \
    target.h \
    mapviewer.h \
    dialog.h \
    loc_widget.h \
    motion.h \
    params_groupbox.h \
    locators_ctrl.h

FORMS    += widget.ui

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../alglib/release/ -lalglib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../alglib/debug/ -lalglib
else:unix: LIBS += -L$$PWD/../alglib/ -lalglib

INCLUDEPATH += $$PWD/../alglib/src
DEPENDPATH += $$PWD/../alglib/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../alglib/release/libalglib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../alglib/debug/libalglib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../alglib/release/alglib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../alglib/debug/alglib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../alglib/libalglib.a
