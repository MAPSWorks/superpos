#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
using namespace std;

inline bool findRootTime(double& t, const double& x,
                         const double& v, const double& a)
{
  double t1, t2, D;

  D = v*v - 4*a*x;

  if (D < 0) return false;

  if (a < 1e-6) {
    if (v < 1e-6) return false;
    t = x / v;
  }
  else {
    t1 = (-v - sqrt(D)) / (2*a);
    t2 = (-v + sqrt(D)) / (2*a);

    if ((t1 < 0) && (t2 < 0)) return false;

    if (t1 < 0) t = t2;
    else if (t2 < 0) t = t1;
    else t = std::min(t1, t2);
  }

  return true;
}


#endif // TOOLS_H
