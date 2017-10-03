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
    /// Обработчики нажатия кнопок
    void beginAddTraj();
    void endAddTraj();
    void deleteTraj();

    /// Обработчик "накалывания" точки новой траектории
    void addTrajPoint(const QMouseEvent*, QPointF);

    /// Изменение данных в модели (программой или пользователем) (!!!)
    void onDataChanged(QModelIndex, QModelIndex, QVector<int>);

    /// Обработчик "кликания" по траектории на карте
    void onTrajClicked(int);

    /// Обработчик выбора траектории в tree_view
    void onTrajSelected(QModelIndex);

  private:
    TreeModel *trajs_model;
    QTreeView  tree_view;

    Trajectories trajs;
    PointsVector points_vector;

    Mapviewer  *map_viewer;
    LineString *linestring;

    QPushButton pbBeginAddTraj,
                pbEndAddTraj,
                pbDelTraj;
};

#endif // TRAJSCTRL_H
