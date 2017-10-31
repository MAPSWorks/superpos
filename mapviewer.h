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
#include <QMapControl/point.h>

#include <QMapControl/imagepoint.h>
#include <QMapControl/circlepoint.h>
#include <QMapControl/linestring.h>

#include <iostream>

#include "locator.h"
#include "target.h"
//#include "trajs_ctrl.h"
#include "trajectory.h"

using namespace qmapcontrol;

class Mapviewer : public QWidget
{
    Q_OBJECT

public:
    Mapviewer(QWidget* parent = 0);
    ~Mapviewer();

    void setTrajsContainer(Trajectories* tr) {trajs = tr;}

    void setView(const QPointF& c) {mc->setView(c);}
    void updateLocators(Locators*);
    void updateLocAzimuths(Locators*);
    void updateTargets(Targets*);
    void resetView(Locators*);

    void updatePixmapAzim(int, int);

    void addTraj(LineString * ls) { trajlayer->addGeometry(ls); }
    void clearTrajLayer() { trajlayer->clearGeometries(); }
    void updateTrajLayer() { mc->update(); }

    void addTarget(Point * t) { targlayer->addGeometry(t); }

    int getZoomLevel() { return mapadapter->adaptedZoom(); }

public slots:
    void updateTraj(int);
    void deleteTraj(int);
    void deleteAllTrajs();
    void updateTrajs();
    void selectTraj(int);
    void selectTarget(int);

signals:
    void mouseEventCoordinate(const QMouseEvent* e, const QPointF p);
    void trajClicked(int);
    void targetClicked(int);

protected slots:
    void updateViewLabels(const QPointF&, int);
    void onMouseEventCoordinate(const QMouseEvent*, const QPointF p);

    void onTrajClicked(Geometry* g, QPoint);
    void onTargetClicked(Geometry* g, QPoint);
    void onUpdateRequest()  {std::cout << "onUpdateRequest" << std::endl;}

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;
    Layer* loclayer;
    Layer* targlayer;
    Layer* trajlayer;
    Layer* azimlayer;
    Layer* notes;
    QLabel* zoomlabel;
    QPixmap pixmap_azim;

    Trajectories *trajs;

    QPen * penActiveTraj,
         * penInactiveTraj;

    void addZoomButtons();
    void addZoomLabel();

protected:
    virtual void resizeEvent(QResizeEvent * event);
    virtual void paintEvent(QPaintEvent * event);
};

#endif
