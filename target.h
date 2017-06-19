#ifndef TARGET_H
#define TARGET_H

#include <QPointF>
#include <QTime>

#include <chrono>

#include "trajectory.h"

using namespace std;


class Target
{
  public:
    Target(BaseTrajectory*);
    ~Target() {}

    void start();
    QPointF getCoords();

  private:
    double getTimeDelta();

    BaseTrajectory *traj;

    chrono::time_point<chrono::system_clock> startTime;
};
typedef std::vector<Target> Targets;

#endif // TARGET_H
