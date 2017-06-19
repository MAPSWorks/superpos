#include "target.h"

#include <iostream>

Target::Target(BaseTrajectory *tr):
  traj(tr)
{

}

void Target::start()
{
  startTime = chrono::system_clock::now();
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
  return traj -> getCoords(getTimeDelta(), 0.001, 0.0005);
}
