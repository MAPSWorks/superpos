#include "mapviewer.h"

#include <QPushButton>
#include <QVBoxLayout>

#include <iostream>

#define COORDS(h,m,s) (h+m/60+s/3600)

using namespace std;

/*!
 * \example mapviewer.cpp
 * This application is just a simple map viewer. A Mapadapter is created (OpenStreetmaps) 
 * and added to a layer. The layer is given to the MapControl.
 * Two Buttons are available to adjust the zoom level. If the window is
 * resized the map widget will adjust its size.
 * 
 * You can find this example here: QMapControl/Samples/Mapviewer
 * \image html sample_mapviewer.png "screenshot"
 */

Mapviewer::Mapviewer(QWidget *parent):
  QWidget(parent)
{
    // create MapControl
    mc = new MapControl(this);

    mc->resize(this->size());

    mc->showScale(true);

    // create mapadapter, for mainlayer and overlay
    mapadapter = new GoogleMapAdapter(GoogleMapAdapter::satellite);

    // create a layer with the mapadapter and type MapLayer
    mainlayer = new MapLayer("OpenStreetMap-Layer", mapadapter);

    // add Layer to the MapControl
    mc->addLayer(mainlayer);
    mc->setDisabled(true);

    //addZoomButtons();

    mc->setView(QPointF(COORDS(34.0, 59.0, 23.0), COORDS(56.0,  8.0, 41.0)));
    mc->setZoom(14);
}

Mapviewer::~Mapviewer()
{
}

void Mapviewer::addZoomButtons()
{
    // create buttons as controls for zoom
    QPushButton* zoomin = new QPushButton("+");
    QPushButton* zoomout = new QPushButton("-");
    zoomin->setMaximumWidth(50);
    zoomout->setMaximumWidth(50);

    connect(zoomin, SIGNAL(clicked(bool)),
            mc, SLOT(zoomIn()));
    connect(zoomout, SIGNAL(clicked(bool)),
            mc, SLOT(zoomOut()));

    // add zoom buttons to the layout of the MapControl
    QVBoxLayout* innerlayout = new QVBoxLayout;
    innerlayout->addWidget(zoomin);
    innerlayout->addWidget(zoomout);
    mc->setLayout(innerlayout);
}

// resize the widget 
void Mapviewer::resizeEvent ( QResizeEvent * event )	
{
  mc->resize(event->size());
}
