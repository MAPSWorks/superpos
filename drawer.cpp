#include "drawer.h"
#include <iostream>

using namespace std;

Drawer::Drawer():
  item(NULL)
{
}

Drawer::~Drawer()
{

}

void Drawer::init()
{
  scene.setSceneRect(0, 0, 800, 600);

  locators[0].init(QPoint(100,0), 0);
  locators[1].init(QPoint(400,0), 0);

  locators[0].addBackground("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_1_fileRLS_FFT_001.b");
  locators[1].addBackground("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_4_fileRLS_FFT_001.b");

  for (int i = 0; i < LOCATORS_NUM; ++i)
    locators[i].updatePixmap();

  targets.push_back(Target(QPoint(0,0), QPoint(20,20)));

  time.start();

  emit updateScreen();
}

void Drawer::process()
{
  QPoint crd = targets[0].getCoords(0.001*time.elapsed());

  scene.clear();

  // Фоновая обстановка по данным локаторов:
  for (int i = 0; i < LOCATORS_NUM; ++i) {
    pixmap = locators[i].getPixmap();
    QGraphicsItem *itemPixmap = scene.addPixmap(pixmap);
    itemPixmap->setOpacity(0.5);
    itemPixmap->setFlag(QGraphicsItem::ItemIsMovable);
    itemPixmap->setTransformOriginPoint(QPoint(pixmap.width()/2, pixmap.height()/2));
    itemPixmap->setPos(locators[i].getCenter());
    itemPixmap->setRotation(locators[i].getAngle0());
  }

  // Текущее направление локаторов:
  for (int i = 0; i < LOCATORS_NUM; ++i) {
    QPoint cnt = locators[i].getCenter() + QPoint(200,200);
    double phi = locators[i].getNextPhi();

    cout << phi << " ";
    if (i == 1) cout << endl;

    double L = 200;
    QGraphicsItem *itemLine = scene.addLine(QLineF(0,0,L,L),
                                            QPen(Qt::red, 2));
    itemLine->setPos(cnt);
    itemLine->setRotation(phi);
  }

  // Цель:
  item = scene.addEllipse(-5,-5, 10,10, QPen(), QBrush(Qt::white));
  item->setPos(crd);

  emit updateScreen();
}
