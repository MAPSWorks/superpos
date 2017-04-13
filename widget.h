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
    void resetToDefault();

    void startImit();
    void stopImit();

  private:
    void updateGroupBox();

    Ui::Widget *ui;
    Mapviewer *mv;

    QSpinBox *sb_first_discr;
    QSpinBox *sb_last_discr;
    QSpinBox *sb_min_ampl;
    QCheckBox *cb_auto_update;

    QTimer timer;

    Locators locators;
    Targets targets;
};

#endif // WIDGET_H
