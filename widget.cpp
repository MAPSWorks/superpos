#include "widget.h"
#include "ui_widget.h"

#include "locator.h"
#include "dialog.h"

#include <iostream>

using namespace std;

Widget::Widget():
  ui(new Ui::Widget)
{
  ui->setupUi(this);

  locators.clear();

  locators.push_back(Locator());
  locators.back().init(QPointF(COORDS(34.0, 59.0, 44.07), COORDS(56.0,  8.0, 49.83)),
            "/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_1_fileRLS_FFT_001.b", 501); // 44 град

/*  locators.push_back(Locator());
  locators.back().init(QPointF(COORDS(34.0, 58.0, 50.20), COORDS(56.0,  8.0, 29.59)),
            "/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_4_fileRLS_FFT_001.b", 1115); // 98 град
*/
 // targets.push_back(Target(QPointF(COORDS(34.0, 59.0, 39.304), COORDS(56.0,  8.0, 42.764)),
  targets.push_back(Target(QPointF(COORDS(34.0, 59.0, 39.304), COORDS(56.0,  8.0, 42.764)),
                           0.03 * QPointF((COORDS(34.0, 58.0, 56.451) - COORDS(34.0, 59.0, 39.304)),
                                          COORDS(56.0,  8.0, 32.691) - COORDS(56.0,  8.0, 42.764))));

  mv = new Mapviewer(this);
  mv->setGeometry(10,10,650,650);
  mv->setView(locators.front().getCenter());
  mv->updatePixmapAzim(DISCR_NUM*SCALE*METERS_IN_DISCR, 0);

  connect(ui->pbAddLoc, SIGNAL(released()), this, SLOT(addLocator()));
  connect(ui->pbDelLoc, SIGNAL(released()), this, SLOT(deleteLocators()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateLocators()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateTabWidget()));
  connect(ui->pbReset,  SIGNAL(released()), this, SLOT(optimizeView()));
  connect(ui->pbStartImit, SIGNAL(released()), SLOT(startImit()));
  connect(ui->pbStopImit,  SIGNAL(released()), SLOT(stopImit()));
  connect(&timer, SIGNAL(timeout()), this, SLOT(updateTargets()));

  updateTabWidget();
  updateGroupBox();
  updateLocators();
}

Widget::~Widget()
{
  delete mv;
  delete ui;
}

void Widget::addLocator()
{
  InputDialog* pInputDialog = new InputDialog;
  if (pInputDialog->exec() == QDialog::Accepted) {
    bool ok1, ok2;
    double lat = pInputDialog->latitude().toDouble(&ok1);
    double lon = pInputDialog->longitude().toDouble(&ok2);
    QString fname = pInputDialog->filename();

    if (!ok1 || !ok2) {
      QMessageBox::information(0, "Добавление РЛС", "Неверный формат координат.");
      return;
    }

    locators.push_back(Locator());
    locators.back().init(QPointF(lat, lon), fname.toStdString().c_str(), 0);

    updateLocators();
    updateTabWidget();
    QMessageBox::information(0, "Добавление РЛС", "Локатор успешно добавлен!");
  }
  delete pInputDialog;
}

void Widget::deleteLocators()
{
  unsigned idx = ui->tabWidget->currentIndex();

  if (idx < locators.size()) {
    Locators::iterator it = locators.begin();
    for (unsigned i = 0; i < idx; ++i) it++;
    locators.erase(it);
    QMessageBox::information(0, "Удаление РЛС",
                            "Локатор " + QString::number(idx+1) + " успешно удален!");
  }

  updateLocators();
  updateTabWidget();
}

void Widget::updateLocators()
{
  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it) {
    it->setFirstDiscr(sb_first_discr->value());
    it->setLastDiscr(sb_last_discr->value());
    it->setMinAmpl(sb_min_ampl->value());
    it->setColorInvert(cb_invert_color->checkState());
  }

  mv->updateLocators(&locators);
}

void Widget::updateTargets()
{
  mv->updateTargets(&targets);
  mv->updateLocAzimuths(&locators);

  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it) {
    it->writeToFile(targets);
  }
}

void Widget::updateTabWidget()
{
  int i = 0;
  int cur = ui->tabWidget->currentIndex();
  ui->tabWidget->clear();

  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it, ++i) {
    ui->tabWidget->addTab(&it->getLocatorWidget(), "РЛС " + QString::number(i+1));
  }
  if (cur <= i)
    ui->tabWidget->setCurrentIndex(cur);
}

void Widget::optimizeView()
{
  mv->resetView(&locators);
}

void Widget::startImit()
{
  timer.start(DELTA_T * 1000);
  for (Targets::iterator it = targets.begin(); it != targets.end(); ++it) {
    it->start();
  }
  unsigned idx = 1;
  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it, ++idx) {
    QString name = "RLS_" + QString::number(idx) + "_Data.b";
    it->start();
    it->setOutFile(name.toStdString().c_str());
  }
}

void Widget::stopImit()
{
  cout << __PRETTY_FUNCTION__ << endl;
  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it) {
    it->closeFile();
  }
  timer.stop();
}

void Widget::updateGroupBox()
{
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
  ui->groupBox->setLayout(vbox);
}
