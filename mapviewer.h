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

using namespace qmapcontrol;
class Mapviewer : public QWidget
{
    Q_OBJECT

public:
    Mapviewer(QWidget* parent = 0);

    ~Mapviewer();

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;
    Layer* notes;

    void addZoomButtons();

protected:
    virtual void resizeEvent ( QResizeEvent * event );
};

#endif
