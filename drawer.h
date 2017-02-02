#ifndef DRAWER_H
#define DRAWER_H

#include <QThread>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <iostream>
#include <vector>

#include "locator.h"
#include "target.h"

#define LOCATORS_NUM 2

class Drawer : public QObject
{
  Q_OBJECT
  signals:
    void updateScreen();

  public:
    Drawer();
    ~Drawer();

    void init();
    QGraphicsScene& getScene() {return scene;}

  public slots:
    void process();

  private:
    QPixmap pixmap;
    QGraphicsScene scene, background;
    QTime time;
    QGraphicsItem *item;

    Locator locators[LOCATORS_NUM];
    std::vector<Target> targets;
};

#endif // DRAWER_H
