#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>
#include <map>

#include <QPointF>

typedef std::vector<QPointF> PointsVector;

class BaseTrajectory
{
  public:
    BaseTrajectory(PointsVector);

    virtual QPointF getCoords(double, double, double) = 0;

    const PointsVector& getPoints() { return points; }
    void setPoints(PointsVector& pv) { points = pv; }

    int getID() { return id; }
    void setID(int i) { id = i; }

  protected:
    PointsVector points;
    int id;
};

class LinearTrajectory : public BaseTrajectory
{
  public:
    LinearTrajectory(PointsVector);

    QPointF getCoords(double t, double v, double a);
};

typedef std::map<int, BaseTrajectory*> Trajectories;

#endif // TRAJECTORY_H
