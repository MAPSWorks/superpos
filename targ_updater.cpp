#include "targ_updater.h"

BaseTargUpdater::BaseTargUpdater(double beg, double dur):
  begin_time(beg), duration(dur), next(NULL)
{
}

bool BaseTargUpdater::actual(double t) {
  return ((t > begin_time) &&
          (t < (begin_time + duration )));
}

bool BaseTargUpdater::isChainActual(double t) {
  if (actual(t)) return true;
  if (next) return next->isChainActual(t);
  else return false;
}

void BaseTargUpdater::rebuildOnTime(double t) {
  begin_time = t;
  if (next) next->rebuildOnTime(t + duration);
}

void BaseTargUpdater::shiftTimeForDelta(double t) {
  begin_time += t;
  if (next) next->shiftTimeForDelta(t);
}

void BaseTargUpdater::add(BaseTargUpdater* n) {
  if (next) next->add(n);
  else      next = n;
}

QPointF BaseTargUpdater::getCoords(double t) {
  if (next) return next->getCoords(t);
  else return QPointF();
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
    if (TrajTools::abs(dir) < 10e-6)
      c = coords0;
    else
      c = coords0 + dir / TrajTools::abs(dir) * (vel * (t-begin_time) + acc * (t-begin_time) / 2);

#if 0
    cout << __PRETTY_FUNCTION__ << "Beg = " << begin_time << ", dur = " << duration
                                << ", Coords: " << c.x() << ", " << c.y() << endl;
#endif
  }
  else {
    if (next)
      c = BaseTargUpdater::getCoords(t);
    else
      c = coords0 + dir / TrajTools::abs(dir) * (vel * duration + acc * duration / 2);
  }

  return c;
}
