#ifndef TARG_UPDATER_H
#define TARG_UPDATER_H

#include <QPointF>
#include <QTime>

#include <chrono>

#include <tools.h>

class BaseTargUpdater
{
public:
  BaseTargUpdater(double, double);
  virtual ~BaseTargUpdater() {}

  bool actual(double t);
  bool isChainActual(double t);
  void rebuildOnTime(double t);

  void add(BaseTargUpdater* n);

  virtual QPointF getCoords(double t);

protected:
  double begin_time,
         duration;

  BaseTargUpdater *next;
};

class LinearTargUpdater : public BaseTargUpdater
{
public:
  LinearTargUpdater(double, double, QPointF, QPointF, double, double);
  ~LinearTargUpdater() {
    if (next) delete next;
  }

  QPointF getCoords(double);

private:
  QPointF coords0,
          dir;
  double  vel,
          acc;
};

#endif // TARG_UPDATER_H
