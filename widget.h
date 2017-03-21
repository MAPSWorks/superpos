#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QtMath>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "drawer.h"
#include "mapviewer.h"

namespace Ui {
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

  protected:
     void paintEvent(QPaintEvent *);
     void wheelEvent(QWheelEvent *);

  public:
    Widget();
    ~Widget();

  private:
    Ui::Widget *ui;
    Drawer drawer;
    QTimer timer;
    QGraphicsScene *scene;
    QGraphicsView  *view;

  public slots:
    void onUpdateScreen() { view->update(); /*cout << "update" << endl;*/ }
};

#endif // WIDGET_H
