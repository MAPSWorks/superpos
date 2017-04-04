#include "mapviewer.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLine>

#include <iostream>

#define COORDS(h,m,s) (h+m/60+s/3600)

using namespace std;

/*!
  Класс обеспечивает работу с MapControl, а также управляет размещением локаторов и целей
*/

Mapviewer::Mapviewer(QWidget *parent):
  QWidget(parent)
{
 // create MapControl
  mc = new MapControl(this);

  mc->showScale(true);

  mc->setBackgroundRole(QPalette::Shadow);
  mc->setAutoFillBackground(true);

  mapadapter = new GoogleMapAdapter(GoogleMapAdapter::satellite);
  mainlayer = new MapLayer("GoogleMapLayer", mapadapter);

  mc->addLayer(mainlayer);

  //mc->setHidden(true);
  //mc->setDisabled(true);

  addZoomButtons();
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
  mc->setZoom(14);

  loclayer = new GeometryLayer("Locators", mapadapter);
  mc->addLayer(loclayer);

  targlayer = new GeometryLayer("Targets", mapadapter);
  mc->addLayer(targlayer);

  //updateLocators();
  //updateTargets();
}

Mapviewer::~Mapviewer()
{
}

void Mapviewer::updateLocators(std::list<Locator> *locators)
{
  if (loclayer == NULL) return;

  loclayer->clearGeometries();

  if (locators->empty()) {
    mc->update();
    return;
  }

  Point *rls[10];
  int i = 0;
  QPointF cnt;
  for (Locators::iterator it = locators->begin(); it != locators->end(); ++it) {
    QTransform tr;
    it->updatePixmap();
    tr.rotate(it->getAngle0()).scale(0.71,0.71);
    rls[i] = new Point(it->getCenter().x(),
                      it->getCenter().y(),
                      it->getPixmap().transformed(tr),
                      "rls");
    loclayer->addGeometry(rls[i]);
    i++;
    cout << "Mapviewer::addGeometry " << it->getCenter().x()
                               << " " << it->getCenter().y() << endl;
    cnt += it->getCenter();
  }
  cnt /= locators->size();

  mc->setView(cnt);
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

