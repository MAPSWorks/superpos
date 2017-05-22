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
//  mapadapter = new GoogleMapAdapter();
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

  connect(mc, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(onMouseEventCoordinate(const QMouseEvent*,QPointF)));
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

  QPointF crd = targets->front().getCoords();
  Point *targ = new Point(crd.x(), crd.y(), QPixmap("./pub.png"));

  targlayer->addGeometry(targ);
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

    Point *azim = new Point(cnt.x(), cnt.y(),
                      pixmap_azim,
                      "azim");

    targlayer->addGeometry(azim);
  }

  Geocentric earth(Constants::WGS84_a(), Constants::WGS84_f());

  QPointF cnt(COORDS(34.0, 59.0, 23.0), COORDS(56.0,  8.0, 41.0));

  QPointF loc = locators->front().getCenter();
  double phi = locators->front().getPhi();

  double lat0 = loc.x(),
         lon0 = loc.y();
  LocalCartesian proj(lat0, lon0, 0);

  double x, y, z, lat, lon, h;
  double L = 1000;

  x = -L * sin(phi * GRAD_TO_RAD);
  y = L * cos(phi * GRAD_TO_RAD);
 // x = 216;
 // y = -51;
  z = 0;
  proj.Reverse(x,y,z,lat,lon,h);

  //double R = sqrt(x*x + y*y);

  double targ_phi = 180.0 / 3.14 * atan(- y / x);
  if (x < 0) targ_phi += 180.0;

/*
  cout << "Phi: " << phi << " " << targ_phi << endl;
  cout << "Local: " << x << " " << y << " " << z << " " << R << endl;
  cout << "Geo:   " << lat << " " << lon << " " << h << endl;
*/
  // proj.Forward(x,y,z,lat,lon,h);


/*
  proj.Forward(x,y,z,lat,lon,h);
  cout << lat << " " << lon << endl;

  proj.Forward(x,y,z,lat,lon,h);
  cout << lat << " " << lon << endl;

  proj.Forward(x,y,z,lat,lon,h);
  cout << lat << " " << lon << endl;

  proj.Forward(x,y,z,lat,lon,h);
  cout << lat << " " << lon << endl;
*/


  QPixmap pxm(10,10);
  pxm.fill(Qt::red);
  Point *targ = new Point(lat, lon, pxm);

  targlayer->addGeometry(targ);
/*
  QPixmap pxm2(10,10);
  pxm.fill(Qt::green);
  Point *targ2 = new Point(COORDS(34.0, 59.0, 20.045), COORDS(56.0,  8.0, 47.994), pxm2);

  targlayer->addGeometry(targ2);
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

