#ifndef TARGET_H
#define TARGET_H

#include <QPointF>

class Target
{
  public:
    Target(QPointF, QPointF);
    QPointF getCoords(double);

  private:
    double startTime;
    QPointF coord0,
            vel;
};

#endif // TARGET_H
