#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QRadioButton>
#include <QtMath>
#include <list>

#include "mapviewer.h"
#include "params_groupbox.h"
#include "locators_ctrl.h"
#include "targets_ctrl.h"
#include "trajs_ctrl.h"


namespace Ui {
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

  public:
    Widget();
    ~Widget();

  public slots:
    void updateLocators();
    void updateTargets();

    void optimizeView();

    void startImit();
    void stopImit();

  private:
    Ui::Widget *ui;
    Mapviewer *mv;

    QTimer timer;

    ParamsGroupBox params;
    LocatorsCtrl   locators_ctrl;
    TrajsCtrl      trajs_ctrl;
    TargetsCtrl    targets_ctrl;
};

#endif // WIDGET_H
