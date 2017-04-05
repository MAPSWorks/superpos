#ifndef LOCATOR_H
#define LOCATOR_H

#include <QPainter>
#include <QPixmap>

#include <vector>
#include <list>
#include <iostream>

#include "parser.h"
#include "target.h"
#include "loc_widget.h"

#define DATA_NUM_ONE_ROUND 750  ///< Количество сообщений, составляющее один обзор
#define DELTA_T            1.0  ///< Период обновления экрана
#define TIME_ONE_ROUND     2.0  ///< Время одного обзора каждого локатора
#define DISCR_NUM          2500 ///< Число отображаемых квантов дальности
#define METERS_IN_DISCR    0.79 ///< Метров в одном кванте
#define SCALE              0.2  ///< Пикселей на один метр

#define COORDS(h,m,s) (h+m/60+s/3600)

typedef std::vector<DATA_PACKAGE_AD> DataCont;

class Locator
{
  public:
    Locator();
    Locator(const Locator& l) {*this = l;}
    Locator& operator=(const Locator&) {return *this;}
    virtual ~Locator();

    void init(QPointF, const char*, int);
    void updatePixmap();
    void writeToFile(Targets&);
    void closeFile() {out_file.close(); std::cout << "closed" << std::endl;}

    const QPixmap&  getPixmap() {return pixmap;}
    const QPointF&  getCenter() {return center;}
    void     setAngle0(double a) { loc_wgt.setAngle0(a); }
    double   getAngle0() { return loc_wgt.getAngle0(); }
    void     setOpacity(double op) { loc_wgt.setOpacity((int)(op*100)); }
    double   getOpacity() { return (double)loc_wgt.getOpacity() / 100.0; }
    void     setRoundsNum(unsigned r) { loc_wgt.setRoundsNum(r); }
    unsigned getRoundsNum() { return loc_wgt.getRoundsNum(); }

    double   getNextPhi();

    LocWidget& getLocatorWidget()  {return loc_wgt;}

    LocWidget loc_wgt;

  private:
    //Locator& operator=(const Locator&);

    Parser   parser;
    DataCont data;
    QPixmap  pixmap;
    DataCont::iterator it_data;
    std::ofstream      out_file;

    QPointF  center;  // Координаты центра
    double   angle0,  // Смещение азимута
             phi;     // Текущий азимут
    double   opacity; // Прозрачность при отрисовке
};
typedef std::list<Locator> Locators;

#endif // LOCATOR_H
