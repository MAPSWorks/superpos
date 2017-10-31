#include "target.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

Target::Target(BaseTrajectory *tr):
  traj(tr), vel(0), acc(0),
  delay(0), isActive(false)
{

}

void Target::start()
{
  startTime = chrono::system_clock::now();
}

void Target::startLater(double d)
{
  delay = d;
  start();
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
  return traj -> getCoords(getTimeDelta(), vel, acc);
}

void Target::reset()
{

  return;
}
