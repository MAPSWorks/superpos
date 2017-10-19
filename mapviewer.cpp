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
  mc = new MapControl(this);

  mc->setBackgroundRole(QPalette::Shadow);
  mc->setAutoFillBackground(true);

  mapadapter = new GoogleMapAdapter(GoogleMapAdapter::satellite);
//  mapadapter = new GoogleMapAdapter();
  mainlayer = new MapLayer("GoogleMapLayer", mapadapter);
  mc->addLayer(mainlayer);

  penActiveTraj = new QPen(QColor(0,255,0, 150), 5);
  penInactiveTraj = new QPen(QColor(0,255,0, 100), 3);

  addZoomButtons();
  mc->setZoom(DEFAULT_ZOOM_LEVEL);

  loclayer = new GeometryLayer("Locators", mapadapter);
  mc->addLayer(loclayer);

  trajlayer = new GeometryLayer("Trajs", mapadapter);
  mc->addLayer(trajlayer);

  targlayer = new GeometryLayer("Targets", mapadapter);
  mc->addLayer(targlayer);

  mc->showScale(true);

  connect(mc, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(onMouseEventCoordinate(const QMouseEvent*,QPointF)));

  connect(trajlayer, SIGNAL(geometryClicked(Geometry*, QPoint)),
          this, SLOT(onTrajClicked(Geometry*, QPoint)));
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

  for (Targets::iterator it = targets->begin(); it != targets->end(); ++it) {
    QPointF crd = it -> getCoords();
    QPixmap pxm(16,16);
    pxm.fill(Qt::red);
    Point *targ = new Point(crd.x(), crd.y(), pxm);
    targlayer->addGeometry(targ);
  }
}

void Mapviewer::updateTrajs()
{
  if (trajlayer == NULL) return;

  trajlayer->clearGeometries();

  for (Trajectories::iterator it = trajs->begin(); it != trajs->end(); ++it) {
    LineString * ls = new LineString();
    ls->setPen(penInactiveTraj);
    PointsVector pv = (*it).second->getPoints();
    for (PointsVector::iterator it_p = pv.begin(); it_p != pv.end(); ++it_p) {
      ls->addPoint(new CirclePoint(it_p->x(), it_p->y(), 10, QString(),
                                   Point::Middle, penInactiveTraj));
    }
    trajlayer->addGeometry(ls);
  }
}

void Mapviewer::updateTraj(int idx)
{
  if (trajlayer == NULL) return;
  if (idx >= trajs->size()) return;

  QList<Geometry*> &geom = trajlayer->getGeometries();

  if (idx >= geom.size()) return;

  LineString * ls = new LineString();
  PointsVector pv = ((*trajs)[idx])->getPoints();
  ls->setPen(penInactiveTraj);

  for (PointsVector::iterator it_p = pv.begin(); it_p != pv.end(); ++it_p) {
    cout << "x, y: " << it_p->x() << ", " << it_p->y() << endl;

    ls->addPoint(new CirclePoint(it_p->x(), it_p->y(), 10, QString(),
                                 Point::Middle, penInactiveTraj));
  }

  geom.replace(idx, ls);

  mc->update();
}

void Mapviewer::deleteTraj(int idx)
{
  if (trajlayer == NULL) return;

  QList<Geometry*> &geom = trajlayer->getGeometries();
  if (idx >= geom.size()) return;

  geom.removeAt(idx);

  for(int i = 0; i < geom.size(); ++i)
    geom[i]->setName(QString::number(i+1));

  mc->update();
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

void Mapviewer::onTrajClicked(Geometry* g, QPoint)
{
  int idx = g->name().toInt() - 1;
  selectTraj(idx);
  emit trajClicked(idx);
}

void Mapviewer::selectTraj(int idx)
{
  QList<Geometry*> &geom = trajlayer->getGeometries();
  for(int i = 0; i < geom.size(); ++i)
    geom[i]->setPen(penInactiveTraj);

  if (idx < geom.size())
    geom[idx]->setPen(penActiveTraj);

  mc->update();
}

void Mapviewer::onMouseEventCoordinate(const QMouseEvent* e, const QPointF p)
{
  emit mouseEventCoordinate(e, p);
}

void Mapviewer::resizeEvent ( QResizeEvent * event )	
{
  mc->resize(event->size());
}

void Mapviewer::paintEvent(QPaintEvent*) {

}

