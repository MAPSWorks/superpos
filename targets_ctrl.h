#ifndef TARGETSCTRL_H
#define TARGETSCTRL_H

#include <QWidget>
#include <QTabWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QPointF>

#include "target.h"
#include "trajs_ctrl.h"

class TargetsCtrl : public QWidget
{
    Q_OBJECT
  public:
    explicit TargetsCtrl(QWidget *parent = 0);

    Targets& getTargets() { return targets; }

    void setMapViewer(Mapviewer * mv);
    void setTrajsCtrl(TrajsCtrl * t) {trajs_ctrl = t;}

    /// Загрузка и сохранение сценариев
    void loadJSON(const QJsonObject &json);
    void saveJSON(QJsonObject &json);

    void createUpdaters();

  signals:
    void eventUpdate();

  public slots:
    void addTarget();
    void deleteTarget();

    /// Изменение данных в модели (программой или пользователем) (!!!)
    void onDataChanged(QModelIndex, QModelIndex, QVector<int>);

    /// Обработчик "кликания" по цели на карте
    void onTargetClicked(int);

    /// Обработчик выбора цели в tree_view
    void onTargetSelected(QModelIndex);

    /// Установка зацикливания движения всех целей
    void setLoopAll(int);

  private:
    void addTarg(Target);
    void deleteAllTargets();

    TreeModel *targs_model;
    QTreeView  tree_view;

    Targets    targets;

    Mapviewer  *map_viewer;
    TrajsCtrl* trajs_ctrl;

    QPushButton pbAddTarg,
                pbDelTarg;
};

#endif // TARGETSCTRL_H
