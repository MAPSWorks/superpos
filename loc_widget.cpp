#include "loc_widget.h"

#include <iostream>

using namespace std;

LocWidget::LocWidget(QWidget *parent) : QWidget(parent)
{
  mainLayout = new QGridLayout;

  // Инициализация виджетов
  sb_rounds_num.setRange(1, 5);
  sb_rounds_num.setValue(1);
  sb_angle0.setRange(-179, 180);
  sb_angle0.setValue(0);
  dsb_speed.setRange(-1.0, 1.0);
  dsb_speed.setSingleStep(0.05);
  dsb_speed.setValue(1/2.5);
  sb_linepos0.setRange(0, 4096);
  sb_linepos0.setValue(0);
  sl_opacity.setOrientation(Qt::Horizontal);
  sl_opacity.setRange(0, 100);
  sl_opacity.setValue(100);

  unsigned i(2);

  mainLayout->addWidget(new QLabel("Смещение севера (0-4095): "), i, 0);
  mainLayout->addWidget(&sb_linepos0, i, 1);
  ++i;

  mainLayout->addWidget(new QLabel("Старт вращения, град: "), i, 0);
  mainLayout->addWidget(&sb_angle0, i, 1);
  ++i;

  mainLayout->addWidget(new QLabel("Скорость вращения, 1/с: "), i, 0);
  mainLayout->addWidget(&dsb_speed, i, 1);
  ++i;

  mainLayout->addWidget(new QLabel("N обзоров: "), i, 0);
  mainLayout->addWidget(&sb_rounds_num, i, 1);
  ++i;

  mainLayout->addWidget(new QLabel("Прозрачность: "), i, 0);
  mainLayout->addWidget(&sl_opacity, i, 1);
  ++i;

  mainLayout->addWidget(new QLabel("Запись ВКЛ: "), i, 0);
  mainLayout->addWidget(&cb_do_write, i, 1);

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
