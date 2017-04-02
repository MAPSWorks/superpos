#include "widget.h"
#include "ui_widget.h"

#include <iostream>

using namespace std;

Widget::Widget():
  ui(new Ui::Widget)
{
  ui->setupUi(this);

  mv = new Mapviewer(this);
  mv->setGeometry(10,10,700,500);

  sb_loc_num = new QSpinBox(this);
  sb_loc_num->setGeometry(800,50, 50,25);
  sb_loc_num->setValue(2);

  for (int i = 0; i < LOCATORS_NUM; ++i) {
    tb_filename[i] = new QTextBrowser(this);
    tb_filename[i]->setGeometry(720,100+i*30, 150,25);

    sb_init_az[i] = new QSpinBox(this);
    sb_init_az[i]->setGeometry(890,100+i*30, 50,25);

    sb_scan_num[i] = new QSpinBox(this);
    sb_scan_num[i]->setGeometry(950,100+i*30, 50,25);
  }
  pb_update = new QPushButton(this);
  pb_update->setGeometry(800, 300, 100,25);
  pb_update->setText("Update");

  connect(pb_update, SIGNAL(released()), mv, SLOT(updateLocators()));
  connect(&timer, SIGNAL(timeout()), mv, SLOT(updateTargets()));
  timer.start(DELTA_T * 1000);
}

Widget::~Widget()
{
  delete mv;
  delete ui;
}
