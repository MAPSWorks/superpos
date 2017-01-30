#ifndef DRAWER_H
#define DRAWER_H

#include <QThread>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <iostream>
#include <vector>

#include "locator.h"

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
    QGraphicsScene scene;

    Locator locators[LOCATORS_NUM];
};

#endif // DRAWER_H
