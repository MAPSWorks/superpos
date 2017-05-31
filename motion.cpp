#include "motion.h"

#include <iostream>;

using namespace std;

BaseMotionModel::BaseMotionModel()
{
}

void BaseMotionModel::start()
{
  startTime = chrono::system_clock::now();
}

double BaseMotionModel::getTimeDelta()
{
  chrono::time_point<chrono::system_clock> cur = std::chrono::system_clock::now();
  double elaps_ns = chrono::duration_cast<std::chrono::nanoseconds>
                           (cur-startTime).count();
  double t = elaps_ns * 1e-9;

  return t;
}

// Линейная модель
LinearMotionModel::LinearMotionModel(QPointF c0, QPointF v):
  coord0(c0), vel(v)
{
}

QPointF LinearMotionModel::getCoords()
{
  return coord0 + vel * getTimeDelta();
}

// Модель движения по ломаной
PoligonalMotionModel::PoligonalMotionModel(PointsVector p, double v):
  points(p), vel(v)
{
  cout << "Points number = " << points.size() << endl;
}

QPointF PoligonalMotionModel::getCoords()
{
  double dt = getTimeDelta();
  double t = 0;
  double t_prev = 0;

  unsigned i = 0;

  if (points.size() == 1)
    return points[0];

  while (i < points.size() - 1) {
    QPointF d = points[i+1] - points[i];
    double abs_d = sqrt(d.x()*d.x() + d.y()*d.y());

    // cout << "abs_d = " << abs_d << endl;

    t_prev = t;
    t += (abs_d) / vel;
    if (dt < t) {
      coords = points[i] + vel * (d / abs_d) * (getTimeDelta() - t_prev);
      break;
    }
    ++i;
  }

  // cout << dt << " " << t << " " << coords.x() << " " << coords.y() << endl;

  return coords; // coord0 + vel * getTimeDelta();
}


/*
// Модель движения по кривой, задаваемой набором точек
SplineMotionModel::SplineMotionModel(PointsVector p, double v):
  points(p), vel(v)
{

  spline1dinterpolant c;

  real_1d_array t,x,y,d;
  double _x[] = {0, 2, 1},
         _y[] = {2, 1, 3},
         _d[] = {0, 0, 0};
  x.setcontent(3, _x);
  y.setcontent(3, _y);
  d.setcontent(3, _d);

  spline1dbuildhermite(x,y,d,c);
  spline1dbuildhermite(x,y,d,c);


}

QPointF SplineMotionModel::getCoords()
{
  double dt = getTimeDelta();



  double v = spline1dcalc(c, dt);


  //cout << dt << " " << t << " " << coords.x() << " " << coords.y() << endl;

  return coords; // coord0 + vel * getTimeDelta();
}
*/
