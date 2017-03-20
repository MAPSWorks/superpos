#ifndef MAPVIEWER_H
#define MAPVIEWER_H
#include <QApplication>
#include <QMainWindow>

#include <QMapControl/mapcontrol.h>
#include <QMapControl/osmmapadapter.h>
#include <QMapControl/maplayer.h>

using namespace qmapcontrol;
class Mapviewer : public QWidget
{
    Q_OBJECT

public:
    Mapviewer(QWidget *parent = 0);

    ~Mapviewer();

private:
    MapControl* mc;
    MapAdapter* mapadapter;
    Layer* mainlayer;

    void addZoomButtons();

protected:
    virtual void resizeEvent ( QResizeEvent * event );
};

#endif
