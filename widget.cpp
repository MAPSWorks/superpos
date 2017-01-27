#include "widget.h"
#include "ui_widget.h"

#include <iostream>

using namespace std;

Widget::Widget():
  ui(new Ui::Widget)
{
  ui->setupUi(this);

  view = new QGraphicsView(this);
  view -> setGeometry(20,20,800,600);
  view -> setScene(&drawer.getScene());

  connect(&timer, SIGNAL(timeout()), &drawer, SLOT(process()));
  timer.start(4000);
}

Widget::~Widget()
{
  delete ui;
}

void Widget::paintEvent(QPaintEvent*) {
  /* void */
}

void Widget::wheelEvent(QWheelEvent *event)
{
   switch(event->modifiers())
   {
     case Qt::ControlModifier: {
       if (event->delta() > 0) {
         view->scale(1.1, 1.1);
       }
       else {
         view->scale(0.9, 0.9);
       }
     }
     default:
       break;
   }
   event->accept();
}
