#include "params_groupbox.h"

#include <QGridLayout>
#include <QLabel>

ParamsGroupBox::ParamsGroupBox()
{

}

void ParamsGroupBox::init(QWidget* parent, const QRect& geom)
{
  setParent(parent);
  setGeometry(geom);

  sb_first_discr = new QSpinBox();
  sb_first_discr->setMinimum(0);
  sb_first_discr->setMaximum(1000);
  sb_first_discr->setValue(100);
  sb_last_discr = new QSpinBox();
  sb_last_discr->setMinimum(1001);
  sb_last_discr->setMaximum(4096);
  sb_last_discr->setValue(2500);
  sb_min_ampl = new QSpinBox();
  sb_min_ampl->setMinimum(0);
  sb_min_ampl->setMaximum(1000);
  sb_min_ampl->setValue(5);
  cb_invert_color = new QCheckBox();

  QGridLayout *vbox = new QGridLayout;
  vbox->addWidget(sb_first_discr, 0,0);
  vbox->addWidget(new QLabel("Начальный квант дальности"), 0,1);
  vbox->addWidget(sb_last_discr, 1,0);
  vbox->addWidget(new QLabel("Конечный квант дальности"), 1,1);
  vbox->addWidget(sb_min_ampl, 2,0);
  vbox->addWidget(new QLabel("Мин. отображаемая амплитуда"), 2,1);
  vbox->addWidget(cb_invert_color, 3,0);
  vbox->addWidget(new QLabel("Инвертировать цвет"), 3,1);
  setLayout(vbox);
}

CommonParams ParamsGroupBox::getCommonParams()
{
  CommonParams p;

  p.first_discr = sb_first_discr->value();
  p.last_discr  = sb_last_discr->value();
  p.min_ampl    = sb_min_ampl->value();
  p.is_color_invert = cb_invert_color->checkState();

  return p;
}
