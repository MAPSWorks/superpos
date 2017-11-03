#ifndef TOOLS_H
#define TOOLS_H

#include <QPointF>

#include <iostream>
using namespace std;

namespace TrajTools {
  inline double abs(QPointF p) {
    return sqrt(p.x()*p.x() + p.y()*p.y());
  }


  inline bool findRootSqrWithCheck(double& t, const double a,
                           const double b, const double c)
  {
    double t1, t2, D;

    D = b*b - 4*a*c;

    if (D < 0) return false;

    if (a < 1e-6) {
      if (b < 1e-6) return false;
      t = - c / b;
    }
    else {
      t1 = (-b - sqrt(D)) / (2*a);
      t2 = (-b + sqrt(D)) / (2*a);

      if ((t1 < 0) && (t2 < 0)) return false;

      if (t1 < 0) t = t2;
      else if (t2 < 0) t = t1;
      else t = std::min(t1, t2);
    }
    return true;
  }

  inline bool findDurationWithCheck(double &delta_t, const double vel,
                           const double acc, QPointF first, QPointF last)
  {
    QPointF d = last - first;
    double abs_d = sqrt(d.x()*d.x() + d.y()*d.y());

    if (abs_d < 10e-6) {
      delta_t = 0;
      return true;
    }

    return findRootSqrWithCheck(delta_t, acc/2, vel, -abs_d);
  }
}



#endif // TOOLS_H
