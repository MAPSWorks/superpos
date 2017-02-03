#ifndef LOCATOR_H
#define LOCATOR_H

#include <QPainter>
#include <QPixmap>
#include <vector>

#include "parser.h"

#define DATA_NUM_ONE_ROUND 750
#define DELTA_T 0.1
#define TIME_ONE_ROUND 5.0

typedef std::vector<DATA_PACKAGE_AD> DataCont;

class Locator
{
  public:
    Locator();

    void init(QPoint, double);
    void addBackground(const char*);
    void addTarget();
    void updatePixmap();

    const QPixmap& getPixmap() {return pixmap;}
    const QPoint&  getCenter() {return center;}
    double   getAngle0() {return angle0;}
    double   getNextPhi();

    void setDeltaT(double dt) {deltaT = dt;}

  private:
    DataCont data;
    QPixmap  pixmap;
    Parser   parser;

    QPoint center; // Координаты центра
    double angle0, // Смещение азимута
           phi;    // Текущий азимут
    double deltaT; // Период поступления данных
};

#endif // LOCATOR_H
