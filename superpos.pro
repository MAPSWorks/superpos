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
    params_groupbox.cpp \
    locators_ctrl.cpp \
    trajectory.cpp \
    targets_ctrl.cpp \
    trajs_ctrl.cpp \
    #trajs_model.cpp \
    treeitem.cpp \
    treemodel.cpp \
    targ_updater.cpp

HEADERS  += widget.h \
    parser.h \
    rls_header.h \
    locator.h \
    target.h \
    mapviewer.h \
    dialog.h \
    loc_widget.h \
    params_groupbox.h \
    locators_ctrl.h \
    trajectory.h \
    tools.h \
    targets_ctrl.h \
    trajs_ctrl.h \
    #trajs_model.h \
    treeitem.h \
    treemodel.h \
    targ_updater.h

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
