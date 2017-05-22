#ifndef MOTION_H
#define MOTION_H

#include <QPointF>

#include <vector>
#include <chrono>

//#include "../alglib/interpolation.h"

//using namespace alglib;
using namespace std;

class BaseMotionModel
{
  public:
    BaseMotionModel();

    void   start();
    double getTimeDelta();

    virtual QPointF getCoords() = 0;

  protected:
    chrono::time_point<chrono::system_clock> startTime;
};

class LinearMotionModel: public BaseMotionModel
{
  public:
    LinearMotionModel(QPointF, QPointF);

    QPointF getCoords();

  private:
    QPointF coord0,
            vel;
};

typedef std::vector<QPointF> PointsVector;

// Модель движения по ломаной
class PoligonalMotionModel: public BaseMotionModel
{
  public:
    PoligonalMotionModel(PointsVector, double);

    QPointF getCoords();

  private:
    PointsVector points;
    double       vel;
    QPointF      coords; // Текущие координаты
};

/*
// Модель движения по кривой, задаваемой набором точек
class SplineMotionModel: public BaseMotionModel
{
  public:
    SplineMotionModel(PointsVector, double);

    QPointF getCoords();

  private:
    PointsVector points;
    double       vel;

    spline1dinterpolant sx, sy;

};
*/

#endif // MOTION_H
