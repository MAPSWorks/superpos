#include "targ_updater.h"

BaseTargUpdater::BaseTargUpdater(double beg, double dur):
  begin_time(beg), duration(dur), next(NULL)
{
}

LinearTargUpdater::LinearTargUpdater(double beg, double dur,
                      QPointF c0, QPointF d, double v, double a):
   BaseTargUpdater(beg, dur), coords0(c0), dir(d), vel(v), acc(a)
{
}

QPointF LinearTargUpdater::getCoords(double t)
{
  QPointF c;
  if (actual(t)) {
    // TODO: Сделать нормальные скорости!!!
    c = coords0 + dir / TrajTools::abs(dir) * (vel * (t-begin_time) + acc * (t-begin_time) / 2);
  }
  else {
    c = BaseTargUpdater::getCoords(t);
  }
  return c;
}
