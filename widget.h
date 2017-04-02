#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPushButton>
#include <QTextBrowser>
#include <QtMath>

#include "mapviewer.h"

namespace Ui {
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

  public:
    Widget();
    ~Widget();

  private:
    Ui::Widget *ui;
    Mapviewer *mv;

    QPushButton *pb_update;
    QSpinBox *sb_loc_num;
    QSpinBox *sb_scan_num[LOCATORS_NUM];
    QSpinBox *sb_init_az[LOCATORS_NUM];
    QTextBrowser *tb_filename[LOCATORS_NUM];

    QTimer timer;


};

#endif // WIDGET_H
