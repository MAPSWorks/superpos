#ifndef TARGET_H
#define TARGET_H

#include <QPoint>

class Target
{
  public:
    Target(QPoint, QPoint);
    QPoint getCoords(double);

  private:
    double startTime;
    QPoint coord0,
           vel;
};

#endif // TARGET_H
