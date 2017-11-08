#ifndef TARGET_H
#define TARGET_H

#include "targ_updater.h"
#include "trajectory.h"

using namespace std;

typedef chrono::time_point<chrono::system_clock> Time;

/// Класс, описывающий цель и её поведение
/// Trajectory - траектория (маршрут движения), хранит набор точек
/// Updater - цепочка из отрезков, должен быть установлен перед началом имитации
class Target
{
  public:
    Target(BaseTrajectory*);
    ~Target() {}

    void start();
    void startLater(double);

    bool isExistingNow();

    QPointF getCoords();

    void setDelay(double t) { delay = t; }

    void setStartTime(Time t) { startTime = t; }
    Time getStartTime() { return startTime; }

    void setIsActive(bool a) { isActive = a; }
    bool getIsActive() { return isActive; }

    void setIsLooped(bool l) { isLooped = l; }
    bool getIsLooped() { return isLooped; }

    int getTrajID() { return traj->getID(); }
    int getPointsNum() { return traj->getPoints().size(); }

    void setUpdater(BaseTargUpdater *u) { upd = u; }

    void shiftTimeForDelta(double d) { upd->shiftTimeForDelta(d); }
    void reset();

  private:
    double getTimeDelta();

    BaseTrajectory  *traj;
    BaseTargUpdater *upd;

    double delay;

    bool   isActive;
    bool   isLooped;

    Time startTime;
};
typedef std::vector<Target> Targets;

#endif // TARGET_H
