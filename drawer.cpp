#include "drawer.h"
#include <iostream>

using namespace std;

Drawer::Drawer()
{
  mv = new Mapviewer();
  mv->setGeometry(-1000, -1000, 2000, 2000);
}

Drawer::~Drawer()
{
  delete mv;
}

void Drawer::init()
{
  scene.setSceneRect(-1000, -1000, 2000, 2000);

  opacityRLS[0] = opacityRLS[1] = 0.5;
  opacityMap = 1.0;

  locators[0].init(QPointF( 363.8,  272.7 * -1), 0);
  locators[1].init(QPointF(-566.4, -352.4 * -1), 0);

  locators[0].addBackground("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_1_fileRLS_FFT_001.b");
  locators[1].addBackground("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_4_fileRLS_FFT_001.b");

  targets.push_back(Target(QPointF(281.51, -256.59),
                           QPointF((-458.44-281.51) / 50,  (54.49+256.59) / 50)));

  time.start();

  // Добавляем карту
  proxyMapView = scene.addWidget(mv);
  proxyMapView->setPos(- QPointF(mv->width()/2, mv->height()/2));
  proxyMapView->setTransformOriginPoint(QPointF(mv->width()/2, mv->height()/2));
  proxyMapView->setScale(1.1);

  // Добавляем данные с локаторов
  for (int i = 0; i < LOCATORS_NUM; ++i) {
    locators[i].updatePixmap();
    pixmaps[i] = locators[i].getPixmap();
    itemPixmap[i] = scene.addPixmap(pixmaps[i]);
    cout << "init locator " << i << ": " << itemPixmap[i] << endl;
  }

  // Центр (КПА)
  scene.addEllipse(-5, -5, 10, 10, QPen(Qt::red, 2));
  // ВПП
  scene.addLine(QLineF(SCALE*QPointF(281.51, 54.49 * -1),
                       SCALE*QPointF(-458.44, -256.59 * -1)),
                QPen(Qt::cyan, 2));


  emit updateScreen();
}

void Drawer::process()
{
  QPointF crd = targets[0].getCoords(0.001*time.elapsed());

  //scene.clear();
  //scene.addWidget(mv);


#if 1
  proxyMapView->setOpacity(opacityMap);
#else
  QPixmap mapview("./map.jpg");
  QGraphicsItem *itemMap = scene.addPixmap(mapview);
  itemMap->setOpacity(1.0);
  itemMap->setTransformOriginPoint(QPointF(mapview.width()/2, mapview.height()/2));
  itemMap->setPos(- QPointF(mapview.width()/2, mapview.height()/2));
  itemMap->setScale(5.0);
#endif

  // Фоновая обстановка по данным локаторов:
  for (int i = 0; i < LOCATORS_NUM; ++i) {
    itemPixmap[i]->setOpacity(opacityRLS[i]);
    itemPixmap[i]->setTransformOriginPoint(QPointF(pixmaps[i].width()/2, pixmaps[i].height()/2));
    itemPixmap[i]->setPos((SCALE * locators[i].getCenter() -
                             QPointF(pixmaps[i].width()/2, pixmaps[i].height()/2)));
    itemPixmap[i]->setRotation(locators[i].getAngle0());

    // Для контроля масштабов:
    /*  scene.addEllipse(locators[i].getCenter().x() - 1000*0.79,
                     locators[i].getCenter().y() - 1000*0.79,
                     2000*0.79, 2000*0.79, QPen(Qt::red, 2));
    */
  }


#if 0
  // Текущее направление локаторов:
  for (int i = 0; i < LOCATORS_NUM; ++i) {
    QPointF cnt = locators[i].getCenter();
    double phi = locators[i].getNextPhi();

    // cout << phi << " ";
    // if (i == 1) cout << endl;

    double L = DISCR_NUM * METERS_IN_DISCR;
    QGraphicsItem *itemLine = scene.addLine(QLineF(0,0, L,0),
                                            QPen(Qt::green, 10));
    itemLine->setPos(cnt);
    itemLine->setRotation(phi);
  }

  // Цель:
  item = scene.addEllipse(-20, -20, 40,40, QPen(), QBrush(Qt::white));
  item->setPos(crd);
#endif

  emit updateScreen();
}
