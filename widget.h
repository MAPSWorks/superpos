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

    void addTargetPoint(const QMouseEvent*, QPointF);

    void startImit();
    void stopImit();

    void beginAddTarget();
    void endAddTarget();
    void deleteTargets();

  private:
    Ui::Widget *ui;
    Mapviewer *mv;

    QTimer timer;

    // Временно
    PointsVector points_vector;

    ParamsGroupBox params;
    LocatorsCtrl locators_ctrl;
    Targets targets;
};

#endif // WIDGET_H
