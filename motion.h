#ifndef MOTION_H
#define MOTION_H

#include <QPointF>

#include <vector>
#include <chrono>

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

class PoligonalMotionModel: public BaseMotionModel
{
  public:
    PoligonalMotionModel(PointsVector, double);

    QPointF getCoords();

  private:
    PointsVector points;
    double       vel;
};

#endif // MOTION_H
