#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>

#include <QPointF>

typedef std::vector<QPointF> PointsVector;

class BaseTrajectory
{
  public:
    BaseTrajectory(PointsVector);

    virtual QPointF getCoords(double, double, double) = 0;

  protected:
    PointsVector points;
};

class LinearTrajectory : public BaseTrajectory
{
  public:
    LinearTrajectory(PointsVector);

    QPointF getCoords(double t, double v, double a);
};

#endif // TRAJECTORY_H
