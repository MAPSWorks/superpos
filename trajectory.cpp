#include <iostream>

#include "trajectory.h"
#include "tools.h"

using namespace std;

BaseTrajectory::BaseTrajectory(PointsVector pv):
  points(pv)
{
}

LinearTrajectory::LinearTrajectory(PointsVector pv):
  BaseTrajectory(pv)
{
}

QPointF LinearTrajectory::getCoords(double t, double vel, double acc)
{
  double delta_t(0), t_next(0), t_prev(0);
  unsigned i(0);

  QPointF res = points[0];

  while (i < points.size() - 1) {
    QPointF d = points[i+1] - points[i];
    double abs_d = sqrt(d.x()*d.x() + d.y()*d.y());

    t_prev = t_next;
    vel += acc * t_prev;

    bool can_get_next_point = findRootSqr(delta_t, acc/2, vel, -abs_d);

    if (!can_get_next_point)
      return points[i] + vel * (d / abs_d) * (t - t_prev) / 2;

    t_next += delta_t;

    if (t < t_next) {
      res = points[i] + (d / abs_d) * (vel * (t - t_prev)
                      + acc * (t - t_prev) * (t - t_prev) / 2);
      break;
    }
    res = points.back();

    i++;
  }

  return res;
}
