#include "trajs_ctrl.h"
#include "dialog.h"

#include <QGridLayout>
#include <QLabel>

TrajsCtrl::TrajsCtrl(QWidget *parent) : QWidget(parent)
{
  trajs.clear();

  updateWidget();

  pbBeginAddTraj.setText("Задать точки");
  pbBeginAddTraj.setGeometry(0,0, 100, 20);
  pbEndAddTraj.setText("Добавить тр-ю");
  pbEndAddTraj.setGeometry(0,0, 100, 20);
  pbDelTraj.setText("Удалить тр-ю");
  pbDelTraj.setGeometry(0,0, 100, 20);

  connect(&pbBeginAddTraj, SIGNAL(released()), SLOT(beginAddTraj()));
  connect(&pbEndAddTraj,   SIGNAL(released()), SLOT(endAddTraj()));
  connect(&pbDelTraj,      SIGNAL(released()), SLOT(deleteTraj()));

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->addWidget(&pbBeginAddTraj);
  hLayout->addWidget(&pbEndAddTraj);
  hLayout->addWidget(&pbDelTraj);

  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->addWidget(&tab_wgt);
  vLayout->addLayout(hLayout);

  setLayout(vLayout);
}

void TrajsCtrl::beginAddTraj()
{
  connect(map_viewer, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(addTrajPoint(const QMouseEvent*,QPointF)));
}

void TrajsCtrl::endAddTraj()
{
  disconnect(map_viewer, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(addTrajPoint(const QMouseEvent*,QPointF)));
  if (!points_vector.empty()) {
    trajs.push_back(new LinearTrajectory(points_vector));
    QMessageBox::information(0, "Добавление траектории", "Траектория успешно добавлена!");
  }
  else {
    QMessageBox::information(0, "Добавление траектории", "Ошибка. Не задано ни одной точки.");
  }
  points_vector.clear();

  updateWidget();
}

void TrajsCtrl::deleteTraj()
{
  unsigned idx = tab_wgt.currentIndex();

  if (idx < trajs.size()) {
    Trajectories::iterator it = trajs.begin();
    for (unsigned i = 0; i < idx; ++i) it++;
    trajs.erase(it);
    QMessageBox::information(0, "Удаление траектории",
                            "Траектория " + QString::number(idx+1)
                                          + " успешно удалена!");
  }

  updateWidget();
  emit eventUpdate();
}

void TrajsCtrl::addTrajPoint(const QMouseEvent* e, QPointF p)
{
  if (e->type() == QEvent::MouseButtonPress) {
    cout << "Point " << p.x() << " " << p.y() << endl;
    points_vector.push_back(p);
  }
}

void TrajsCtrl::updateWidget()
{
  int i = 0;
  int cur = tab_wgt.currentIndex();
  tab_wgt.clear();

  for (Trajectories::iterator it = trajs.begin(); it != trajs.end(); ++it, ++i) {
    tab_wgt.addTab(new QLabel("Что-то"), "Тр." + QString::number(i+1));
  }

  if (cur <= i)
    tab_wgt.setCurrentIndex(cur);
}
