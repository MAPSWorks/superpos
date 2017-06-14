#ifndef PARAMSGROUPBOX_H
#define PARAMSGROUPBOX_H

#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>

class ParamsGroupBox : public QGroupBox
{
  public:
    ParamsGroupBox();

    void init();

    unsigned getFirstDiscr()    { return sb_first_discr->value(); }
    unsigned getLastDiscr()     { return sb_last_discr->value(); }
    unsigned getMinAmpl()       { return sb_min_ampl->value(); }
    bool     getIsColorInvert() { return cb_invert_color->checkState(); }

  private:
    QSpinBox *sb_first_discr;
    QSpinBox *sb_last_discr;
    QSpinBox *sb_min_ampl;
    QCheckBox *cb_invert_color;
};

#endif // PARAMSGROUPBOX_H
