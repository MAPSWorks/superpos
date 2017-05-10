#include "motion.h"

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

LinearMotionModel::LinearMotionModel(QPointF c0, QPointF v):
  coord0(c0), vel(v)
{
}

QPointF LinearMotionModel::getCoords()
{
  return coord0 + vel * getTimeDelta();
}
