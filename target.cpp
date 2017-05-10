#include "target.h"

#include <iostream>

Target::Target(BaseMotionModel *m):
  motion(m)
{

}

/*
void Target::start()
{
  startTime = chrono::system_clock::now();
}

QPointF Target::getCoords()
{
  chrono::time_point<chrono::system_clock> cur = std::chrono::system_clock::now();

  double elaps_ns = chrono::duration_cast<std::chrono::nanoseconds>
                           (cur-startTime).count();

  // cout << "Elapsed ns = " << elaps_ns << endl;

  double t = elaps_ns * 1e-9;
  return coord0 + vel * t;
}
*/
