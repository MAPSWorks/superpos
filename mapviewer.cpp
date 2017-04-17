#include "mapviewer.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLine>
#include <QList>

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/GeodesicLine.hpp>

#include <iostream>

#define COORDS(h,m,s) (h+m/60+s/3600)
#define DEFAULT_ZOOM_LEVEL 14
#define MAX_ZOOM_LEVEL 17

using namespace std;
using namespace GeographicLib;

/*!
  Класс обеспечивает работу с MapControl, а также управляет размещением локаторов и целей
*/

Mapviewer::Mapviewer(QWidget *parent):
  QWidget(parent)
{
 // create MapControl
  mc = new MapControl(this);

  mc->setBackgroundRole(QPalette::Shadow);
  mc->setAutoFillBackground(true);

  mapadapter = new GoogleMapAdapter(GoogleMapAdapter::satellite);
  mainlayer = new MapLayer("GoogleMapLayer", mapadapter);
  mc->addLayer(mainlayer);

  //mc->setHidden(true);
  //mc->setDisabled(true);

  addZoomButtons();
  mc->setZoom(DEFAULT_ZOOM_LEVEL);

  loclayer = new GeometryLayer("Locators", mapadapter);
  mc->addLayer(loclayer);

  targlayer = new GeometryLayer("Targets", mapadapter);
  mc->addLayer(targlayer);

  mc->showScale(true);
}

Mapviewer::~Mapviewer()
{
}

void Mapviewer::updateLocators(Locators *locators)
{
  if (loclayer == NULL) return;

  loclayer->clearGeometries();

  double sc = 1.48 / SCALE;

  for (Locators::iterator it = locators->begin(); it != locators->end(); ++it) {
    QTransform tr;
    it->updatePixmap();
    tr.scale(sc, sc);
    Point *rls = new Point(it->getCenter().x(),
                      it->getCenter().y(),
                      it->getPixmap().transformed(tr),
                      "rls");
    rls->setBaselevel(MAX_ZOOM_LEVEL);
    loclayer->addGeometry(rls);
    cout << "add Locator on coords " << it->getCenter().x()
                             << ", " << it->getCenter().y() << endl;
  }

  mc->update();
}

void Mapviewer::updateTargets(Targets* targets)
{
  if (targlayer == NULL) return;

  targlayer->clearGeometries();

  // QTransform tr;
  Point *targ;

  QPointF crd = targets->front().getCoords();

 // targ = new Point(crd.x(), crd.y(), QPixmap("./pub.png"));

 // targlayer->addGeometry(targ);



  /*
    Geodesic geod(Constants::WGS84_a(), Constants::WGS84_f());

    QPointF c0 = locators->front().getCenter();

    const GeodesicLine line = geod.Line(c0.x(), c0.y(), 0);
    double lat, lon;
    line.Position(1000, lat, lon);

    cout << "Position: " << lat << " " << lon << "\n";

    cout << "coordinateToDisplay: "
         << mapadapter->coordinateToDisplay(QPointF(lat, lon)).x() -
            mapadapter->coordinateToDisplay(c0).x() << endl;
  */

/*
  if (targets->front().getTimeElapsed() < 20.0) {
    locators.writeToFile(targets);
  }
  else {
    locators[0].closeFile();
  }
*/
}

void Mapviewer::updatePixmapAzim(int L, int phi)
{
  pixmap_azim = QPixmap(2*L, 2*L);
  pixmap_azim.fill(Qt::transparent);
  QPainter painter(&pixmap_azim);
  painter.translate(pixmap_azim.width()/2, pixmap_azim.height()/2);
  painter.rotate(phi);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
  painter.drawLine(0,0, L,0);
  painter.end();
}

void Mapviewer::updateLocAzimuths(Locators* locators)
{
  for (Locators::iterator it = locators->begin(); it != locators->end(); ++it) {
    const QPointF cnt = it->getCenter();
    const double phi = it->getPhi();

    updatePixmapAzim(2000 / pow(2,mapadapter->currentZoom()), phi);

    Geodesic geod(Constants::WGS84_a(), Constants::WGS84_f());

    //cout << "angle = " << (it_data->data.line_pos.pos * POS_TO_GRAD + getAngle0()) << endl;

    const GeodesicLine line = geod.DirectLine(cnt.x(), cnt.y(), 360.0-phi, 500);
    double lat, lon;
    line.Position(1000, lat,lon);

    Point *targ = new Point(lat, lon, QPixmap("./pub.png"));
    targlayer->addGeometry(targ);


    Point *azim = new Point(cnt.x(), cnt.y(),
                      pixmap_azim,
                      "azim");

    targlayer->addGeometry(azim);
  }
}

void Mapviewer::resetView(Locators *locators)
{
  if (!locators->empty()) {
    QPointF cnt;
    for (Locators::iterator it = locators->begin(); it != locators->end(); ++it)
       cnt += it->getCenter();
     cnt /= locators->size();

     mc->setView(cnt);
  }

  mc->setZoom(DEFAULT_ZOOM_LEVEL);
  mc->update();
}

void Mapviewer::addZoomButtons()
{
  // create buttons as controls for zoom
  QPushButton* zoomin = new QPushButton("+");
  QPushButton* zoomout = new QPushButton("-");
  zoomin->setMaximumWidth(50);
  zoomout->setMaximumWidth(50);

  zoomlabel = new QLabel();
  zoomlabel->setMaximumSize(QSize(100,50));
  zoomlabel->setFont(QFont("Helvetica", 12, 2));

  connect(mc, SIGNAL(viewChanged(const QPointF&, int)),
          this, SLOT(updateViewLabels(const QPointF&, int)));
  connect(zoomin, SIGNAL(clicked(bool)),
          mc, SLOT(zoomIn()));
  connect(zoomout, SIGNAL(clicked(bool)),
          mc, SLOT(zoomOut()));

  // add zoom buttons to the layout of the MapControl
  QVBoxLayout* innerlayout = new QVBoxLayout;
  innerlayout->addWidget(zoomin);
  innerlayout->addWidget(zoomout);
  innerlayout->addWidget(zoomlabel);
  mc->setLayout(innerlayout);
}

void Mapviewer::updateViewLabels(const QPointF &, int)
{
  zoomlabel->setText("Zoom level " +
                      QString::number(mapadapter->adaptedZoom()));
}

void Mapviewer::resizeEvent ( QResizeEvent * event )	
{
  mc->resize(event->size());
}

void Mapviewer::paintEvent(QPaintEvent*) {

}

