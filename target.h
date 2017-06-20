#ifndef TARGET_H
#define TARGET_H

#include <QPointF>
#include <QTime>

#include <chrono>

#include "trajectory.h"

using namespace std;

typedef chrono::time_point<chrono::system_clock> Time;

class Target
{
  public:
    Target(BaseTrajectory*);
    ~Target() {}

    void start();

    bool isExistingNow();

    QPointF getCoords();

    void setVel(double v) { vel = v; }
    double getVel() { return vel; }

    void setAcc(double a) { acc = a; }
    double getAcc() { return acc; }

    void setStartTime(Time t) { startTime = t; }
    Time getStartTime() { return startTime; }

  private:
    double getTimeDelta();

    BaseTrajectory *traj;

    double vel, acc;

    Time startTime;
};
typedef std::vector<Target> Targets;

#endif // TARGET_H
