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
            "/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_1_fileRLS_FFT_001.b", 44);

  locators.push_back(Locator());
  locators.back().init(QPointF(COORDS(34.0, 58.0, 50.20), COORDS(56.0,  8.0, 29.59)),
            "/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_4_fileRLS_FFT_001.b", 98);

  targets.push_back(Target(QPointF(COORDS(34.0, 59.0, 39.304), COORDS(56.0,  8.0, 42.764)),
                           0.03 * QPointF((COORDS(34.0, 58.0, 56.451) - COORDS(34.0, 59.0, 39.304)),
                                           COORDS(56.0,  8.0, 32.691) - COORDS(56.0,  8.0, 42.764))));

  mv = new Mapviewer(this);
  mv->setGeometry(10,10,650,650);
  mv->setView(locators.front().getCenter());

  connect(ui->pbAddLoc, SIGNAL(released()), this, SLOT(addLocator()));
  connect(ui->pbDelLoc, SIGNAL(released()), this, SLOT(deleteLocators()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateLocators()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateTabWidget()));
  connect(ui->pbReset,  SIGNAL(released()), this, SLOT(resetToDefault()));
  //connect(&timer, SIGNAL(timeout()), this, SLOT(updateTargets()));

  timer.start(DELTA_T * 1000);

  updateLocators();
  //updateTargets();
  updateTabWidget();
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
  locators.clear();
  updateLocators();
  updateTabWidget();
}

void Widget::updateLocators()
{
  mv->updateLocators(&locators);
}

void Widget::updateTargets()
{
  mv->updateTargets(&targets);
}

void Widget::updateTabWidget()
{
  int i = 0;
  int cur = ui->tabWidget->currentIndex();
  ui->tabWidget->clear();

  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it, ++i) {
      ui->tabWidget->addTab(&it->getLocatorWidget(), "РЛС " + QString::number(i+1));
      cout << "Update TabWidget for RLS " << i << endl;
  }
  if (cur <= i)
    ui->tabWidget->setCurrentIndex(cur);
}

void Widget::resetToDefault()
{
  mv->resetView(&locators);
  mv->updateLocators(&locators);
}
