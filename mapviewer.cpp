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

  locators[0].init(QPointF(COORDS(34.0, 59.0, 44.07), COORDS(56.0,  8.0, 49.83)), 44);
  locators[1].init(QPointF(COORDS(34.0, 58.0, 50.20), COORDS(56.0,  8.0, 29.59)), 98);

  locators[0].addBackground("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_1_fileRLS_FFT_001.b");
  locators[1].addBackground("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_4_fileRLS_FFT_001.b");

  targets.push_back(Target(QPointF(COORDS(34.0, 59.0, 39.304), COORDS(56.0,  8.0, 42.764)),
                           QPointF(-0.0001,  0)));

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

  QList<QPointF> coords;
  for (int i = 0; i < LOCATORS_NUM; ++i)
    coords << locators[i].getCenter();

  cout << "coords = " << coords.size() << " "
                      << coords.first().rx() << " "
                      << coords.first().ry() << " "
                      << coords.last().rx() << " "
                      << coords.last().ry() << " " << endl;

  mc->setView(locators[0].getCenter());
  mc->setZoom(14);

  loclayer = new GeometryLayer("Locators", mapadapter);
  mc->addLayer(loclayer);

  targlayer = new GeometryLayer("Targets", mapadapter);
  mc->addLayer(targlayer);

  updateLocators();
  updateTargets();
}

Mapviewer::~Mapviewer()
{
}

void Mapviewer::updateLocators()
{
  if (loclayer == NULL) return;

  locators[0].updatePixmap();
  locators[1].updatePixmap();

  QTransform tr[LOCATORS_NUM];
  Point *rls[LOCATORS_NUM];

  loclayer->clearGeometries();

  for (int i = 0; i < LOCATORS_NUM; ++i) {
    tr[i].rotate(locators[i].getAngle0()).scale(0.71,0.71);
    rls[i] = new Point(locators[i].getCenter().x(),
                        locators[i].getCenter().y(),
                        locators[i].getPixmap().transformed(tr[i]),
                        "rls" + i);
    loclayer->addGeometry(rls[i]);
  }
}

void Mapviewer::updateTargets()
{
  if (targlayer == NULL) return;

  targlayer->clearGeometries();

  QTransform tr;
  Point *targ;

  QPointF crd = targets[0].getCoords();

  targ = new Point(crd.x(), crd.y(), QPixmap("./pub.png"));
  targlayer->addGeometry(targ);

  if (targets[0].getTimeElapsed() < 30.0) {
    locators[0].writeToFile(targets);
  }
  else {
    locators[0].closeFile();
  }

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

