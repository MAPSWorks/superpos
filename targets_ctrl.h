#ifndef TARGETSCTRL_H
#define TARGETSCTRL_H

#include <QWidget>
#include <QTabWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QPointF>

#include "target.h"

class TargetsCtrl : public QWidget
{
    Q_OBJECT
  public:
    explicit TargetsCtrl(QWidget *parent = 0);

  signals:
    void eventUpdate();

  public slots:
    void addTarget();
    void deleteTarget();
    void updateWidget();

  private:
    Targets targets;
    QTabWidget tab_wgt;

    QPushButton pbAddTarg,
                pbDelTarg;
};

#endif // TARGETSCTRL_H
