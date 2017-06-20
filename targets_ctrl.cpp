#include "targets_ctrl.h"
#include "dialog.h"

#include <QGridLayout>

TargetsCtrl::TargetsCtrl(QWidget *parent) : QWidget(parent)
{
  targets.clear();

  // targets.push_back();

  updateWidget();

  pbAddTarg.setText("Добавить цель");
  pbAddTarg.setGeometry(0,0, 100, 20);
  pbDelTarg.setText("Удалить цель");
  pbDelTarg.setGeometry(0,0, 100, 20);

  connect(&pbAddTarg, SIGNAL(released()), SLOT(addTarget()));
  connect(&pbDelTarg, SIGNAL(released()), SLOT(deleteTarget()));

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->addWidget(&pbAddTarg);
  hLayout->addWidget(&pbDelTarg);

  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->addWidget(&tab_wgt);
  vLayout->addLayout(hLayout);

  setLayout(vLayout);
}

void TargetsCtrl::addTarget()
{
  TargetDialog* pDialog = new TargetDialog(3);
  if (pDialog->exec() == QDialog::Accepted) {
    bool ok1, ok2;
    unsigned traj_num = pDialog->traj_num();
    double vel = pDialog->velocity().toDouble(&ok1);
    double acc = pDialog->accel().toDouble(&ok2);

    if (!ok1 || !ok2) {
      QMessageBox::information(0, "Добавление цели", "Неверно введены данные.");
      return;
    }

    Target targ(new LinearTrajectory(PointsVector()));
    //targ.setTraj(trajs[traj_num]);
    targ.setVel(vel);
    targ.setAcc(acc);
    targets.push_back(targ);

    updateWidget();
    emit eventUpdate();

    QMessageBox::information(0, "Добавление цели", "Цель успешно добавлена!");
  }
  delete pDialog;
}

void TargetsCtrl::deleteTarget()
{
  unsigned idx = tab_wgt.currentIndex();

  if (idx < targets.size()) {
    Targets::iterator it = targets.begin();
    for (unsigned i = 0; i < idx; ++i) it++;
    targets.erase(it);
    QMessageBox::information(0, "Удаление цели",
                            "Цель " + QString::number(idx+1) + " успешно удалена!");
  }

  updateWidget();
  emit eventUpdate();
}

void TargetsCtrl::updateWidget()
{
  int i = 0;
  int cur = tab_wgt.currentIndex();
  tab_wgt.clear();

 /* for (Targets::iterator it = targets.begin(); it != targets.end(); ++it, ++i) {
    tab_wgt.addTab(&it->getTargetWidget(), "РЛС " + QString::number(i+1));
  }*/

  if (cur <= i)
    tab_wgt.setCurrentIndex(cur);
}
