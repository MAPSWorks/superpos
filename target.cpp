#include "target.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

Target::Target(BaseTrajectory *tr):
  traj(tr), delay(0), isActive(false),
  isLooped(false), upd(NULL)
{
}

void Target::start()
{
  startTime = chrono::system_clock::now();
}

bool Target::isExistingNow()
{
  bool res = chrono::duration_cast<std::chrono::nanoseconds>
               (std::chrono::system_clock::now()-startTime).count()
               > delay;
  return res;
}

double Target::getTimeDelta()
{
  double t;
  chrono::time_point<chrono::system_clock> cur = std::chrono::system_clock::now();
  double elaps_ns = chrono::duration_cast<std::chrono::nanoseconds>
                           (cur-startTime).count();
  t = elaps_ns * 1e-9 - delay;

  if ((t > 10e6) || (t < 0))
    t = 0;

  return t;
}

QPointF Target::getCoords()
{
  QPointF c;

  if (upd) {
    double t = getTimeDelta();
    if (!upd->isChainActual(t)) {
      if (isLooped) upd->rebuildOnTime(t);
    }
    c = upd -> getCoords(t);
  }
  else {
    c = QPointF();
  }
  return c;
}

void Target::reset()
{

  return;
}
