#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
using namespace std;

inline bool findRootSqr(double& t, const double& a,
                         const double& b, const double& c)
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


#endif // TOOLS_H
