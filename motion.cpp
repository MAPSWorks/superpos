#include "motion.h"

#include <iostream>;

using namespace std;

BaseMotionModel::BaseMotionModel()
{
}

void BaseMotionModel::start()
{
  startTime = chrono::system_clock::now();
}

double BaseMotionModel::getTimeDelta()
{
  chrono::time_point<chrono::system_clock> cur = std::chrono::system_clock::now();
  double elaps_ns = chrono::duration_cast<std::chrono::nanoseconds>
                           (cur-startTime).count();
  double t = elaps_ns * 1e-9;

  return t;
}

// Линейная модель
LinearMotionModel::LinearMotionModel(QPointF c0, QPointF v):
  coord0(c0), vel(v)
{
}

QPointF LinearMotionModel::getCoords()
{
  return coord0 + vel * getTimeDelta();
}

// Модель движения по ломаной
PoligonalMotionModel::PoligonalMotionModel(PointsVector p, double v):
  points(p), vel(v)
{
  cout << "Points number = " << points.size() << endl;
}

QPointF PoligonalMotionModel::getCoords()
{
  QPointF res;

  res = points[0] + vel * (points[1] - points[0]) * getTimeDelta();

  return res; // coord0 + vel * getTimeDelta();
}
