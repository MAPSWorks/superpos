#include "widget.h"
#include "ui_widget.h"

#include "locator.h"

#include <iostream>
#include <QToolBox>

using namespace std;

Widget::Widget():
  ui(new Ui::Widget)
{
  ui->setupUi(this);

  mv = new Mapviewer(this);
  mv->setGeometry(10,10,650,650);
  mv->setView(locators_ctrl.getCenter());
  mv->updatePixmapAzim(DISCR_NUM*SCALE*METERS_IN_DISCR, 0);

  connect(ui->pbUpdate, SIGNAL(released()), &locators_ctrl, SLOT(updateTabWidget()));
  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateLocators()));
  connect(ui->pbReset,  SIGNAL(released()), this, SLOT(optimizeView()));
  connect(&locators_ctrl, SIGNAL(eventUpdate()), this, SLOT(updateLocators()));

  // Панель общих параметров
  params.init(this, QRect(0, 0, 340, 200));

  // Управление целями
  connect(ui->pbBeginAddTarg, SIGNAL(released()), SLOT(beginAddTarget()));
  connect(ui->pbEndAddTarg,   SIGNAL(released()), SLOT(endAddTarget()));
  connect(ui->pbDeleteTargs,  SIGNAL(released()), SLOT(deleteTargets()));

  // Управление имитацией
  connect(ui->pbStartImit, SIGNAL(released()), SLOT(startImit()));
  connect(ui->pbStopImit,  SIGNAL(released()), SLOT(stopImit()));

  // Объединяем панели в ToolBox
  QToolBox *tb = new QToolBox(this);
  tb->addItem(&locators_ctrl, "Локаторы");
  tb->addItem(&params, "Общие параметры");
  tb->setGeometry(670, 20, 350, 400);


  connect(&timer, SIGNAL(timeout()), this, SLOT(updateTargets()));

  updateLocators();
}

Widget::~Widget()
{
  delete mv;
  delete ui;
}

void Widget::updateLocators()
{
  locators_ctrl.updateCommonParams(params.getCommonParams());
  mv->updateLocators(&locators_ctrl.getLocators());
}

void Widget::updateTargets()
{
  mv->updateTargets(&targets);
  mv->updateLocAzimuths(&locators_ctrl.getLocators());

  for (Locators::iterator it = locators_ctrl.getLocators().begin();
                          it != locators_ctrl.getLocators().end(); ++it)
  {
    it->writeToFile(targets);
  }
}

void Widget::optimizeView()
{
  mv->resetView(&locators_ctrl.getLocators());
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
  for (Locators::iterator it = locators_ctrl.getLocators().begin();
                          it != locators_ctrl.getLocators().end(); ++it, ++idx) {
    QString name = "RLS_" + QString::number(idx) + "_Data.b";
    it->start();
    it->setOutFile(name.toStdString().c_str());
  }
}

void Widget::stopImit()
{
  for (Locators::iterator it = locators_ctrl.getLocators().begin();
                          it != locators_ctrl.getLocators().end(); ++it) {
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
    targets.push_back(Target(new LinearTrajectory(points_vector)));
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
