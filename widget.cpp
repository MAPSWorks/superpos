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

  drawer.init();

  connect(ui->spinBoxR1, SIGNAL(valueChanged(int)), &drawer, SLOT(setAngle0_RLS1(int)));
  connect(ui->spinBoxR2, SIGNAL(valueChanged(int)), &drawer, SLOT(setAngle0_RLS2(int)));




  connect(&timer, SIGNAL(timeout()), &drawer, SLOT(process()));
  timer.start(DELTA_T * 1000);
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
