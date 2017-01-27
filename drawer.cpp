#include "drawer.h"
#include <iostream>

using namespace std;

Drawer::Drawer()
{
  locators[0].init(QPoint(100,200), 0);
  locators[1].init(QPoint(300,200), 0);

  scene.setSceneRect(0, 0, 800, 600);
}

Drawer::~Drawer()
{

}

void Drawer::init()
{

}

void Drawer::process()
{
  scene.clear();

  for (int i = 0; i < LOCATORS_NUM; ++i) {
    locators[i].updatePixmap();
    pixmap = locators[i].getPixmap();
    scene.addPixmap(pixmap);
  }

  emit updateScreen();
}
