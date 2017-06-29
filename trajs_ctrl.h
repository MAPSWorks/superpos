#ifndef TRAJSCTRL_H
#define TRAJSCTRL_H

#include <QWidget>
#include <QTabWidget>
#include <QPointF>
#include <QMessageBox>
#include <QPushButton>

#include "trajectory.h"
#include "mapviewer.h"

class TrajsCtrl: public QWidget
{
  Q_OBJECT

  public:
    explicit TrajsCtrl(QWidget *parent = 0);

    void setMapViewer(Mapviewer * mv) {map_viewer = mv;}

    Trajectories& getTrajs() { return trajs; }

  signals:
    void eventUpdate();

  public slots:
    void beginAddTraj();
    void endAddTraj();
    void deleteTraj();
    void updateWidget();

    void addTrajPoint(const QMouseEvent*, QPointF);

  private:
    Trajectories trajs;
    QTabWidget tab_wgt;

    PointsVector points_vector;

    Mapviewer *map_viewer;

    QPushButton pbBeginAddTraj,
                pbEndAddTraj,
                pbDelTraj;
};

#endif // TRAJSCTRL_H
