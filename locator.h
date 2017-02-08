#ifndef LOCATOR_H
#define LOCATOR_H

#include <QPainter>
#include <QPixmap>
#include <vector>

#include "parser.h"

#define DATA_NUM_ONE_ROUND 750 ///< Количество сообщений, составляющее один обзор
#define DELTA_T            0.5 ///< Период обновления экрана
#define TIME_ONE_ROUND     10.0 ///< Время одного обзора каждого локатора

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
    void     setAngle0(double a) {angle0 = a;}
    double   getAngle0() {return angle0;}
    double   getNextPhi();

  private:
    DataCont data;
    QPixmap  pixmap;
    Parser   parser;

    QPoint center; // Координаты центра
    double angle0, // Смещение азимута
           phi;    // Текущий азимут
};

#endif // LOCATOR_H
