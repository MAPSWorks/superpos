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
#include <QMapControl/point.h>

#include "locator.h"
#include "target.h"

using namespace qmapcontrol;

class Mapviewer : public QWidget
{
    Q_OBJECT

public:
    Mapviewer(QWidget* parent = 0);
    ~Mapviewer();

    void setView(const QPointF& c) {mc->setView(c);}
    void updateLocators(Locators*);
    void updateLocAzimuths(Locators*);
    void updateTargets(Targets*);
    void resetView(Locators*);

    int getZoomLevel() { return mapadapter->adaptedZoom(); }

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;
    Layer* loclayer;
    Layer* targlayer;
    Layer* notes;
    QLabel* zoomlabel;

    void addZoomButtons();
    void addZoomLabel();

protected slots:
    void updateViewLabels(const QPointF&, int);

protected:
    virtual void resizeEvent(QResizeEvent * event);
    virtual void paintEvent(QPaintEvent * event);
};

#endif
