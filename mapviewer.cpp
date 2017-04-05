#include "mapviewer.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLine>

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/GeodesicLine.hpp>
#include <GeographicLib/GeodesicLineExact.hpp>

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

/*
  QList<QPointF> coords;
  for (int i = 0; i < LOCATORS_NUM; ++i)
    coords << locators[i].getCenter();

  cout << "coords = " << coords.size() << " "
                      << coords.first().rx() << " "
                      << coords.first().ry() << " "
                      << coords.last().rx() << " "
                      << coords.last().ry() << " " << endl;

*/

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

  Point *rls[10];
  int i = 0;
  double sc = 7.4 / pow(2, mc->currentZoom());
  //double sc = 0.9;

  cout << "scale = " << sc << endl;
  for (Locators::iterator it = locators->begin(); it != locators->end(); ++it) {
    QTransform tr;
    it->updatePixmap();

    tr.rotate(it->getAngle0()).scale(sc, sc);
    rls[i] = new Point(it->getCenter().x(),
                      it->getCenter().y(),
                      it->getPixmap().transformed(tr),
                      "rls");
    loclayer->addGeometry(rls[i]);
    i++;
    cout << "Mapviewer::addGeometry " << it->getCenter().x()
                               << " " << it->getCenter().y() << endl;
  }


 /* Geodesic geod(Constants::WGS84_a(), Constants::WGS84_f());

  QPointF c0 = locators->front().getCenter();

  const GeodesicLine line = geod.Line(c0.x(), c0.y(), 0);
  double lat, lon;
  line.Position(1000, lat, lon);

  cout << "Position: " << lat << " " << lon << "\n";

  cout << "coordinateToDisplay: "
       << mapadapter->coordinateToDisplay(QPointF(lat, lon)).x() -
          mapadapter->coordinateToDisplay(c0).x() << endl;
*/

  mc->update();
}

void Mapviewer::updateTargets(Targets* targets)
{
  if (targlayer == NULL) return;

  targlayer->clearGeometries();

  QTransform tr;
  Point *targ;

  QPointF crd = targets->front().getCoords();

  targ = new Point(crd.x(), crd.y(), QPixmap("./pub.png"));
  targlayer->addGeometry(targ);

/*
  if (targets->front().getTimeElapsed() < 20.0) {
    locators.writeToFile(targets);
  }
  else {
    locators[0].closeFile();
  }
*/

/*
    for (int i = 0; i < LOCATORS_NUM; ++i) {
    QPointF cnt = locators[i].getCenter();
    double phi = locators[i].getNextPhi();

    double L = DISCR_NUM * METERS_IN_DISCR;

    QList<Point*> points;
    points << new Point(cnt.rx(), cnt.ry());
    points << new Point(cnt.rx() + L * cos(phi),
                        cnt.ry() + L * sin(phi));

    //LineString* line = new LineString(points, QPen(QBrush(Qt::red), 3));

    //targlayer->addGeometry(line);
  }
*/
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
  zoomlabel->setMaximumSize(QSize(100,100));
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
  //cout << "updateViewLabels" << endl;
}

// resize the widget 
void Mapviewer::resizeEvent ( QResizeEvent * event )	
{
  mc->resize(event->size());
}

void Mapviewer::paintEvent(QPaintEvent*) {
  /* void */

  /*cout << "Mapviewer::paintEvent, rect = " << event->region().boundingRect().bottom() << endl;

  QPainter p;
  p.begin(this);

  p.setRenderHint(QPainter::Antialiasing);
  p.setPen( QPen(Qt::red, 20, Qt::SolidLine) );
  p.drawLine(10,10, 100, 100);

  p.end();
*/
}

