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

    void setAngle0_RLS1(int val) {locators[0].setAngle0(val);}
    void setAngle0_RLS2(int val) {locators[1].setAngle0(val);}

    void setOpacity_RLS1(int val) {opacity[0] = (double) val / 100;}
    void setOpacity_RLS2(int val) {opacity[1] = (double) val / 100;}

  private:
    QPixmap pixmap;
    QGraphicsScene scene, background;
    QTime time;
    QGraphicsItem *item;
    double opacity[LOCATORS_NUM];

    Locator locators[LOCATORS_NUM];
    std::vector<Target> targets;
};

#endif // DRAWER_H
