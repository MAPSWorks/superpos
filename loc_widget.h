#ifndef LOC_WIDGET_H
#define LOC_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QSlider>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>

class LocWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit LocWidget(QWidget *parent = 0);

    QPushButton&  getButtonUpdate()   {return pb_update;}
    QSpinBox&     getSpinboxScanNum() {return sb_rounds_num;}
    QSpinBox&     getSpinboxInitAz()  {return sb_init_az;}
    QSlider&      getOpacitySlider()  {return sl_opacity;}

    void     setAngle0(int a)    { sb_init_az.setValue(a); }
    int      getAngle0()         { return sb_init_az.value(); }

    void     setOpacity(int a)   { sl_opacity.setValue(a); }
    int      getOpacity()        { return sl_opacity.value(); }

    void     setRoundsNum(int a) { sb_rounds_num.setValue(a); }
    unsigned getRoundsNum()      { return sb_rounds_num.value(); }

    void     setDoWrite(bool b)  { cb_do_write.setChecked(b); }
    bool     getDoWrite()        { return cb_do_write.checkState(); }

    void     setFilename(const char*);
    void     setCoords(QPointF&);

  signals:

  public slots:

  private:
    QGridLayout *mainLayout;

    QPushButton pb_update;
    QSpinBox    sb_rounds_num;
    QSpinBox    sb_init_az;
    QSlider     sl_opacity;
    QCheckBox   cb_do_write;
};

#endif // LOC_WIDGET_H
