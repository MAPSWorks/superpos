#ifndef TARGET_H
#define TARGET_H

#include <QPointF>
#include <QTime>

#include <chrono>

using namespace std;

class Target
{
  public:
    Target(QPointF, QPointF);
    ~Target() {}

    void start();
    QPointF getCoords();
    double getTimeElapsed() {return 0.001*time.elapsed();}

  private:
    QTime time;
    QPointF coord0,
            vel;

    chrono::time_point<chrono::system_clock> startTime;
};
typedef std::vector<Target> Targets;

#endif // TARGET_H
