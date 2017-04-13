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
#define DELTA_T            0.3  ///< Период обновления экрана
#define TIME_ONE_ROUND     10.0  ///< Время одного обзора каждого локатора
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
    void setOutFile(const char* name);
    void updatePixmap();
    void writeToFile(Targets&);
    void closeFile() {out_file.close();}

    const QPixmap&  getPixmap() {return pixmap;}
    const QPointF&  getCenter() {return center;}
    void     setLinePos0(double lp) { loc_wgt.setLinePos0(lp); }
    double   getLinePos0() { return loc_wgt.getLinePos0(); }
    void     setAngle0(unsigned a) { loc_wgt.setAngle0(a); }
    double   getAngle0() { return loc_wgt.getAngle0(); }
    void     setOpacity(double op) { loc_wgt.setOpacity((int)(op*100)); }
    double   getOpacity() { return (double)loc_wgt.getOpacity() / 100.0; }
    void     setRoundsNum(unsigned r) { loc_wgt.setRoundsNum(r); }
    unsigned getRoundsNum() { return loc_wgt.getRoundsNum(); }
    void     setFirstDiscr(unsigned f) { first_discr = f; }
    void     setLastDiscr(unsigned l) { last_discr = l; }
    void     setMinAmpl(unsigned a) { min_ampl = a; }

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

    double phi; // Текущий азимут

    QPointF  center;      // Координаты центра
    unsigned first_discr, // Начало вывода (дискр. дальн.)
             last_discr,  // Конец вывода (дискр. дальн.)
             min_ampl;    // Минимальная отображаемая амплитуда
};
typedef std::list<Locator> Locators;

#endif // LOCATOR_H
