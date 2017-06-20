#include "target.h"

#include <iostream>

Target::Target(BaseTrajectory *tr):
  traj(tr), vel(0), acc(0)
{

}

void Target::start()
{
  startTime = chrono::system_clock::now();
}

bool Target::isExistingNow()
{
  return (chrono::system_clock::now() > startTime);
}

double Target::getTimeDelta()
{
  chrono::time_point<chrono::system_clock> cur = std::chrono::system_clock::now();
  double elaps_ns = chrono::duration_cast<std::chrono::nanoseconds>
                           (cur-startTime).count();
  double t = elaps_ns * 1e-9;

  return t;
}

QPointF Target::getCoords()
{
  return traj -> getCoords(getTimeDelta(), vel, acc);
}
