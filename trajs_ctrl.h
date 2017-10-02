#ifndef TRAJSCTRL_H
#define TRAJSCTRL_H

#include <QWidget>
#include <QTabWidget>
#include <QPointF>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeView>

#include "mapviewer.h"
#include "trajs_model.h"
#include "treemodel.h"

class TrajsCtrl: public QWidget
{
  Q_OBJECT

  public:
    explicit TrajsCtrl(QWidget *parent = 0);

    void setMapViewer(Mapviewer * mv);

    Trajectories& getTrajs() { return trajs; }

  signals:
    void eventUpdate();
    void trajSelected(int);

  public slots:
    void beginAddTraj();
    void endAddTraj();
    void deleteTraj();
    void updateWidget();

    void addTrajPoint(const QMouseEvent*, QPointF);
    void onDataChanged(QModelIndex, QModelIndex, QVector<int>);
    void onTrajClicked(int);
    void onTrajSelected(QModelIndex);

  private:
    Trajectories trajs;
    QTabWidget tab_wgt;
    TreeModel *trajs_model;
    QTreeView tree_view;

    PointsVector points_vector;

    Mapviewer *map_viewer;

    LineString* linestring;

    QPushButton pbBeginAddTraj,
                pbEndAddTraj,
                pbDelTraj;
};

#endif // TRAJSCTRL_H
