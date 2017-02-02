#include "target.h"

Target::Target(QPoint c0, QPoint v):
  coord0(c0), vel(v), startTime(0)
{
}

QPoint Target::getCoords(double time)
{
  return coord0 + vel * (time - startTime);
}
