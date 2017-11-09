#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QRadioButton>
#include <QToolBox>
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

class Widget : public QMainWindow
{
  Q_OBJECT

  public:
    Widget();
    ~Widget();

  public slots:
    void updateLocators();
    void updateTargets();

    void optimizeView();

    void loadScenarioJSON();
    void saveScenarioJSON();

    void startImit();
    void pauseImit();
    void stopImit();

  private:
    void setAllEnabled(bool);

    Ui::Widget *ui;
    Mapviewer *mv;

    QTimer timer;

    chrono::time_point<chrono::system_clock> timeBeforePause;

    QToolBox * main_toolbox;
    ParamsGroupBox params;
    LocatorsCtrl   locators_ctrl;
    TrajsCtrl      trajs_ctrl;
    TargetsCtrl    targets_ctrl;
};

#endif // WIDGET_H
