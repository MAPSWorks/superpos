#ifndef PARAMSGROUPBOX_H
#define PARAMSGROUPBOX_H

#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>

struct CommonParams {
  unsigned first_discr;
  unsigned last_discr;
  unsigned min_ampl;
  bool     is_color_invert;
};

class ParamsGroupBox : public QGroupBox
{
  public:
    ParamsGroupBox();

    void init(QWidget*, const QRect&);
    CommonParams getCommonParams();

  private:
    QSpinBox *sb_first_discr;
    QSpinBox *sb_last_discr;
    QSpinBox *sb_min_ampl;
    QCheckBox *cb_invert_color;
};

#endif // PARAMSGROUPBOX_H
