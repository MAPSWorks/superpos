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
#include "locator.h"
#include "params_groupbox.h"

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
    void addLocator();
    void deleteLocators();
    void updateLocators();
    void updateTargets();
    void updateTabWidget();
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

    ParamsGroupBox *params;

    Locators locators;
    Targets targets;
};

#endif // WIDGET_H
