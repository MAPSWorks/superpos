#ifndef TARGET_H
#define TARGET_H

#include <QPointF>
#include <QTime>

#include "motion.h"

using namespace std;


class Target
{
  public:
    Target(BaseMotionModel*);
    ~Target() {}

    void start() { motion -> start(); }
    QPointF getCoords() {return motion -> getCoords();}

  private:
    BaseMotionModel *motion;


};
typedef std::vector<Target> Targets;

#endif // TARGET_H
