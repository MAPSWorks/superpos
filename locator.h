#ifndef LOCATOR_H
#define LOCATOR_H

#include <QPainter>
#include <QPixmap>
#include <vector>
#include <iostream>

#include "parser.h"
#include "target.h"

#define DATA_NUM_ONE_ROUND 750  ///< Количество сообщений, составляющее один обзор
#define DELTA_T            1.0  ///< Период обновления экрана
#define TIME_ONE_ROUND     2.0  ///< Время одного обзора каждого локатора
#define DISCR_NUM          2500 ///< Число отображаемых квантов дальности
#define METERS_IN_DISCR    0.79 ///< Метров в одном кванте
#define SCALE              0.2  ///< Пикселей на один метр

typedef std::vector<DATA_PACKAGE_AD> DataCont;

class Locator
{
  public:
    Locator();
    ~Locator();

    void init(QPointF, double);
    void addBackground(const char*);
    void writeToFile(Targets&);
    void closeFile() {file.close(); std::cout << "closed" << std::endl;}
    void updatePixmap();

    const QPixmap& getPixmap() {return pixmap;}
    const QPointF&  getCenter() {return center;}
    void     setAngle0(double a) {angle0 = a;}
    double   getAngle0() {return angle0;}
    double   getNextPhi();

  private:
    DataCont data;
    QPixmap  pixmap;
    Parser   parser;
    DataCont::iterator it_data;
    std::ofstream      file;

    QPointF center; // Координаты центра
    double angle0, // Смещение азимута
           phi;    // Текущий азимут
};

#endif // LOCATOR_H
