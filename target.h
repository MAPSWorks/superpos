#ifndef TARGET_H
#define TARGET_H

#include <QPointF>
#include <QTime>

class Target
{
  public:
    Target(QPointF, QPointF);
    QPointF getCoords();
    double getTimeElapsed() {return 0.001*time.elapsed();}

  private:
    QTime time;
    double startTime;
    QPointF coord0,
            vel;
};
typedef std::vector<Target> Targets;

#endif // TARGET_H
