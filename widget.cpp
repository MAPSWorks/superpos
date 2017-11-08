#include "widget.h"
#include "ui_widget.h"

#include "locator.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std;

Widget::Widget():
  ui(new Ui::Widget)
{
  ui->setupUi(this);

  mv = new Mapviewer(this);
  mv->setGeometry(10,10,650,650);
  mv->setView(locators_ctrl.getCenter());
  mv->updatePixmapAzim(DISCR_NUM*SCALE*METERS_IN_DISCR, 0);

  trajs_ctrl.setMapViewer(mv);
  mv->setTrajsContainer(&trajs_ctrl.getTrajs());
  targets_ctrl.setTrajsCtrl(&trajs_ctrl);
  targets_ctrl.setMapViewer(mv);

  connect(ui->pbUpdate, SIGNAL(released()), this, SLOT(updateLocators()));
  connect(ui->pbReset,  SIGNAL(released()), this, SLOT(optimizeView()));
  connect(&locators_ctrl, SIGNAL(eventUpdate()), this, SLOT(updateLocators()));

  // Панель общих параметров
  params.init(this, QRect(0, 0, 340, 200));

  // Управление имитацией
  connect(ui->pbStartImit, SIGNAL(released()), SLOT(startImit()));
  connect(ui->pbPauseImit, SIGNAL(released()), SLOT(pauseImit()));
  connect(ui->pbStopImit,  SIGNAL(released()), SLOT(stopImit()));
  connect(ui->cbLoop,      SIGNAL(stateChanged(int)), &targets_ctrl, SLOT(setLoopAll(int)));

  // Управление сценариями (временно)
  connect(ui->pbLoadScenario, SIGNAL(released()), SLOT(loadScenarioJSON()));
  connect(ui->pbSaveScenario,  SIGNAL(released()), SLOT(saveScenarioJSON()));

  // Объединяем панели в ToolBox
  main_toolbox = new QToolBox(this);
  main_toolbox->addItem(&locators_ctrl, "Локаторы");
  main_toolbox->addItem(&trajs_ctrl,    "Траектории");
  main_toolbox->addItem(&targets_ctrl,  "Цели");
  main_toolbox->addItem(&params,        "Общие параметры");
  main_toolbox->setGeometry(670, 20, 350, 500);

  connect(&timer, SIGNAL(timeout()), this, SLOT(updateTargets()));

  updateLocators();

  setAllEnabled(true);
  ui->pbPauseImit->setEnabled(false);
  ui->pbStopImit->setEnabled(false);
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
  mv->updateTargets(&targets_ctrl.getTargets());
  mv->updateLocAzimuths(&locators_ctrl.getLocators());

  for (Locators::iterator it = locators_ctrl.getLocators().begin();
                          it != locators_ctrl.getLocators().end(); ++it)
  {
    it->writeToFile(targets_ctrl.getTargets());
  }
}

void Widget::optimizeView()
{
  mv->resetView(&locators_ctrl.getLocators());
}

void Widget::loadScenarioJSON()
{
  QFile loadFile(QStringLiteral("scenario.json"));

  if (!loadFile.open(QIODevice::ReadOnly)) {
      qWarning("Couldn't open save file.");
      return;
  }

  QByteArray data = loadFile.readAll();
  QJsonDocument loadDoc(QJsonDocument::fromJson(data));
  QJsonObject json = loadDoc.object();

  //locators_ctrl.loadJSON(json);
  trajs_ctrl.loadJSON(json);
  targets_ctrl.loadJSON(json);
}

void Widget::saveScenarioJSON()
{
  QJsonObject json;

  //locators_ctrl.saveJSON(json);
  trajs_ctrl.saveJSON(json);
  targets_ctrl.saveJSON(json);

  QFile saveFile(QStringLiteral("scenario.json"));

  if (!saveFile.open(QIODevice::WriteOnly)) {
      qWarning("Couldn't open save file.");
      return;
  }
  QJsonDocument saveDoc(json);
  saveFile.write(saveDoc.toJson());
  saveFile.close();
}

void Widget::startImit()
{
  targets_ctrl.createUpdaters();
  Targets& targets = targets_ctrl.getTargets();
  if (targets.empty()) {
    QMessageBox::information(0, "Запуск имитации", "Нет целей для имитации.");
    return;
  }

  setAllEnabled(false);
  ui->pbPauseImit->setEnabled(true);
  ui->pbStopImit->setEnabled(true);

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

void Widget::pauseImit()
{
  /*
  if (timer.isActive()) {
    timer.stop();
  }
  else {
    timer.start();
  }
  */
}

void Widget::stopImit()
{
  setAllEnabled(true);

  for (Targets::iterator it = targets_ctrl.getTargets().begin();
                         it != targets_ctrl.getTargets().end(); ++it) {
    it->reset();
  }
  for (Locators::iterator it = locators_ctrl.getLocators().begin();
                          it != locators_ctrl.getLocators().end(); ++it) {
    it->reset();
    it->closeFile();
  }
  timer.stop();
}

void Widget::setAllEnabled(bool e)
{
  mv->setEnabled(e);
  main_toolbox->setEnabled(e);
  ui->pbStartImit->setEnabled(e);
  ui->pbStopImit->setEnabled(e);
  ui->pbPauseImit->setEnabled(e);
  ui->pbReset->setEnabled(e);
  ui->pbUpdate->setEnabled(e);
  ui->pbSaveScenario->setEnabled(e);
  ui->pbLoadScenario->setEnabled(e);
}
