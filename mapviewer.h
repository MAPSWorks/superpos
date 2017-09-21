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

    int getZoomLevel() { return mapadapter->adaptedZoom(); }

public slots:
    void updateTraj(unsigned);
    void deleteTraj(unsigned);
    void updateTrajs();

signals:
    void mouseEventCoordinate (const QMouseEvent* e, const QPointF p);

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;
    Layer* loclayer;
    Layer* targlayer;
    Layer* trajlayer;
    Layer* notes;
    QLabel* zoomlabel;
    QPixmap pixmap_azim;

    Trajectories* trajs;

    void addZoomButtons();
    void addZoomLabel();

protected slots:
    void updateViewLabels(const QPointF&, int);
    void onMouseEventCoordinate(const QMouseEvent* e, const QPointF p)
      { emit mouseEventCoordinate(e, p); }

    void onGeometryClicked(Geometry*, QPoint) {std::cout << "onGeometryClicked" << std::endl;}
    void onUpdateRequest()  {std::cout << "onUpdateRequest" << std::endl;}

protected:
    virtual void resizeEvent(QResizeEvent * event);
    virtual void paintEvent(QPaintEvent * event);
};

#endif
