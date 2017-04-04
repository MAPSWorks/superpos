#include "loc_widget.h"

#include <iostream>

using namespace std;

LocWidget::LocWidget(QWidget *parent) : QWidget(parent)
{
  mainLayout = new QGridLayout;

  // Инициализация виджетов
  sb_rounds_num.setMinimum(1);
  sb_rounds_num.setMaximum(5);
  sb_rounds_num.setValue(1);
  sb_init_az.setMinimum(-179);
  sb_init_az.setMaximum(180);
  sb_init_az.setValue(0);
  sl_opacity.setOrientation(Qt::Horizontal);
  sl_opacity.setMinimum(0);
  sl_opacity.setMaximum(100);
  sl_opacity.setValue(100);

  mainLayout->addWidget(new QLabel("Нач. азимут: "), 2, 0);
  mainLayout->addWidget(&sb_init_az, 2, 1);

  mainLayout->addWidget(new QLabel("N обзоров: "), 3, 0);
  mainLayout->addWidget(&sb_rounds_num, 3, 1);

  mainLayout->addWidget(new QLabel("Прозрачность:"), 4, 0);
  mainLayout->addWidget(&sl_opacity, 4, 1);

  mainLayout->addWidget(new QLabel("Запись ВКЛ:"), 5, 0);
  mainLayout->addWidget(&cb_do_write, 5, 1);

  setLayout(mainLayout);
}

void LocWidget::setFilename(const char* f)
{
  mainLayout->addWidget(new QLabel("Файл: "), 0, 0);
  QTextEdit *te = new QTextEdit(f);
  te->setReadOnly(true);
  mainLayout->addWidget(te, 0, 1);
}

void LocWidget::setCoords(QPointF& c)
{
  mainLayout->addWidget(new QLabel("Координаты:"), 1, 0);
  mainLayout->addWidget(new QLabel(QString::number(c.rx()) + ", " +
                                   QString::number(c.ry())), 1, 1);
}
