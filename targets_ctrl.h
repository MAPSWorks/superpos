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

    void setTrajsCtrl(TrajsCtrl * t) {trajs_ctrl = t;}

  signals:
    void eventUpdate();

  public slots:
    void addTarget();
    void deleteTarget();
    void updateWidget();

  private:
    Targets    targets;
    QTabWidget tab_wgt;

    TrajsCtrl* trajs_ctrl;

    QPushButton pbAddTarg,
                pbDelTarg;
};

#endif // TARGETSCTRL_H
