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
    Ui::Widget *ui;
    Mapviewer *mv;

    QTimer timer;

    Locators locators;
    Targets targets;
};

#endif // WIDGET_H
