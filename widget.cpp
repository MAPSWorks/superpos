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

  Mapviewer mv(view);

  view -> setScene(&drawer.getScene());
  view -> scale(0.12, 0.12);
  // view -> setBackgroundBrush(Qt::black);

  drawer.init();

  connect(ui->spinBoxR1, SIGNAL(valueChanged(int)), &drawer, SLOT(setAngle0_RLS1(int)));
  connect(ui->spinBoxR2, SIGNAL(valueChanged(int)), &drawer, SLOT(setAngle0_RLS2(int)));
  connect(ui->hSlider1, SIGNAL(valueChanged(int)), &drawer, SLOT(setOpacity_RLS1(int)));
  connect(ui->hSlider2, SIGNAL(valueChanged(int)), &drawer, SLOT(setOpacity_RLS2(int)));

  drawer.setAngle0_RLS1(ui->spinBoxR1->value());
  drawer.setAngle0_RLS2(ui->spinBoxR2->value());
  drawer.setOpacity_RLS1(ui->hSlider1->value());
  drawer.setOpacity_RLS2(ui->hSlider2->value());

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
