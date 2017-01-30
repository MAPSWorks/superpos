#include "drawer.h"
#include <iostream>

using namespace std;

Drawer::Drawer()
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


  for (int i = 0; i < LOCATORS_NUM; ++i) {
    locators[i].updatePixmap();
    pixmap = locators[i].getPixmap();
    QGraphicsItem *item = scene.addPixmap(pixmap);
    item->setOpacity(0.5);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setPos(locators[i].getCenter());
  }

  emit updateScreen();
}

void Drawer::process()
{

}
