#include "target.h"

Target::Target(QPointF c0, QPointF v):
  coord0(c0), vel(v), startTime(0)
{
}

QPointF Target::getCoords(double time)
{
  return coord0 + vel * (time - startTime);
}
