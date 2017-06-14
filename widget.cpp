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

/*
  locators.push_back(Locator());
  locators.back().init(QPointF(COORDS(34.0, 58.0, 50.20), COORDS(56.0,  8.0, 29.59)),
            "/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_4_fileRLS_FFT_001.b", 1115); // 98 град
*/

/*
  targets.push_back(Target(new LinearMotionModel(QPointF(COORDS(34.0, 59.0, 39.304), COORDS(56.0,  8.0, 42.764)),
                           0.03 * QPointF((COORDS(34.0, 58.0, 56.451) - COORDS(34.0, 59.0, 39.304)),
                                          COORDS(56.0,  8.0, 32.691) - COORDS(56.0,  8.0, 42.764)))));
*/


//  points_vector.push_back(QPointF(COORDS(34.0, 59.0, 39.304), COORDS(56.0,  8.0, 42.764)));
//  points_vector.push_back(QPointF(COORDS(34.0, 58.0, 56.451), COORDS(56.0,  8.0, 32.691)));
//  points_vector.push_back(QPointF(COORDS(34.0, 58.0, 10.451), COORDS(56.0,  8.0, 32.691)));
//  targets.push_back(Target(new PoligonalMotionModel(pv, 0.002)));

  mv = new Mapviewer(this);
  mv->setGeometry(10,10,650,650);
  mv->setView(locators.front().getCenter());
  mv->updatePixmapAzim(DISCR_NUM*SCALE*METERS_IN_DISCR, 0);

  // Панель общих параметров
  params = new ParamsGroupBox;
  params->setParent(this);
  params->setGeometry(680, 450, 300, 200);
  params->init();

  connect(ui->pbAddLoc, SIGNAL(released()), this, SLOT(addLocator()));
  connect(ui->pbDelLoc, SIGNAL(released()), this, SLOT(deleteLocators()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateLocators()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateTabWidget()));
  connect(ui->pbReset,  SIGNAL(released()), this, SLOT(optimizeView()));

  connect(ui->pbStartImit, SIGNAL(released()), SLOT(startImit()));
  connect(ui->pbStopImit,  SIGNAL(released()), SLOT(stopImit()));
  connect(ui->pbBeginAddTarg, SIGNAL(released()), SLOT(beginAddTarget()));
  connect(ui->pbEndAddTarg,   SIGNAL(released()), SLOT(endAddTarget()));
  connect(ui->pbDeleteTargs,  SIGNAL(released()), SLOT(deleteTargets()));

  connect(&timer, SIGNAL(timeout()), this, SLOT(updateTargets()));

  updateTabWidget();
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
    it->setFirstDiscr(params->getFirstDiscr());
    it->setLastDiscr(params->getLastDiscr());
    it->setMinAmpl(params->getMinAmpl());
    it->setColorInvert(params->getIsColorInvert());
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
  if (targets.empty()) {
    QMessageBox::information(0, "Запуск имитации", "Нет целей для имитации.");
    return;
  }

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
  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it) {
    it->closeFile();
  }
  timer.stop();
}

void Widget::beginAddTarget()
{
  connect(mv, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(addTargetPoint(const QMouseEvent*,QPointF)));
}

void Widget::endAddTarget()
{
  disconnect(mv, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(addTargetPoint(const QMouseEvent*,QPointF)));
  if (!points_vector.empty())
    targets.push_back(Target(new PoligonalMotionModel(points_vector, 0.001)));
  points_vector.clear();

  ui->labelTargNum->setText("Количество целей - " + QString::number(targets.size()));
}

void Widget::deleteTargets()
{
  points_vector.clear();
  targets.clear();
  ui->labelTargNum->setText("Количество целей - " + QString::number(targets.size()));
}

void Widget::addTargetPoint(const QMouseEvent* e, QPointF p)
{
  if (e->type() == QEvent::MouseButtonPress) {
    cout << "Point " << p.x() << " " << p.y() << endl;
    points_vector.push_back(p);
  }
}
