#ifndef MAPVIEWER_H
#define MAPVIEWER_H
#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>

#include <QMapControl/mapcontrol.h>
#include <QMapControl/openaerialmapadapter.h>
#include <QMapControl/osmmapadapter.h>
#include <QMapControl/googlemapadapter.h>
#include <QMapControl/googleapimapadapter.h>
#include <QMapControl/maplayer.h>
#include <QMapControl/geometrylayer.h>
#include <QMapControl/linestring.h>

#include "locator.h"
#include "target.h"

#define LOCATORS_NUM 2

using namespace qmapcontrol;

class Mapviewer : public QWidget
{
    Q_OBJECT

public:
    Mapviewer(QWidget* parent = 0);

    ~Mapviewer();


public slots:
    void updateLocators();
    void updateTargets();

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;
    Layer* loclayer;
    Layer* targlayer;
    Layer* notes;

    Locator locators[LOCATORS_NUM];
    Targets targets;

    void addZoomButtons();


protected:
    virtual void resizeEvent(QResizeEvent * event);
    virtual void paintEvent(QPaintEvent * event);
};

#endif
