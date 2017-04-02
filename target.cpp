#include "target.h"

Target::Target(QPointF c0, QPointF v):
  coord0(c0), vel(v), startTime(0)
{
  time.start();
}

QPointF Target::getCoords()
{
  double t = 0.001 * time.elapsed();
  return coord0 + vel * (t - startTime);
}
