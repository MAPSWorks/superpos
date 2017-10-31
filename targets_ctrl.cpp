#include "targets_ctrl.h"
#include "dialog.h"

#include <QGridLayout>
#include <QJsonObject>
#include <QJsonArray>

TargetsCtrl::TargetsCtrl(QWidget *parent) : QWidget(parent)
{
  QStringList headers;
  headers << tr("Ключ") << tr("Значение");
  targs_model = new TreeModel(headers, QString());

  targets.clear();

  pbAddTarg.setText("Добавить цель");
  pbAddTarg.setGeometry(0,0, 100, 20);
  pbDelTarg.setText("Удалить цель");
  pbDelTarg.setGeometry(0,0, 100, 20);

  connect(&pbAddTarg, SIGNAL(released()), SLOT(addTarget()));
  connect(&pbDelTarg, SIGNAL(released()), SLOT(deleteTarget()));

  tree_view.setModel(targs_model);
  tree_view.setWindowTitle(QObject::tr("Simple Tree Model"));
  tree_view.setItemDelegate(new DoubleSpinBoxDelegate);

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->addWidget(&pbAddTarg);
  hLayout->addWidget(&pbDelTarg);
  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->addWidget(&tree_view);
  vLayout->addLayout(hLayout);
  setLayout(vLayout);
}

void TargetsCtrl::loadJSON(const QJsonObject &json)
{
  disconnect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  deleteAllTargets();

  QJsonArray targArray = json["Targets"].toArray();

  for (int i = 0; i < targArray.size(); ++i) {
    QJsonObject targObject = targArray[i].toObject();

    Trajectories trajs = trajs_ctrl->getTrajs();
    Target t(trajs.at(targObject["Traj ID"].toInt()));
    double start_time = targObject["Start"].toDouble();
    t.setIsActive(true);
    //t.setStartTime(start_time);
    t.setVel(targObject["Vel"].toDouble());
    t.setAcc(targObject["Acc"].toDouble());

    addTarg(t);
  }
  map_viewer->updateTargets(&targets);

  connect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  QMessageBox::information(0, "Добавление целей", "Цели успешно добавлены!");
}

void TargetsCtrl::saveJSON(QJsonObject &json)
{
  QJsonArray targArray;

  for (Targets::iterator it = targets.begin(); it != targets.end(); it++) {
    QJsonObject targObject;

    targObject.insert("Traj ID", it->getTrajID());
    targObject.insert("Vel",  it->getVel());
    targObject.insert("Acc",  it->getAcc());

    targArray.append(targObject);
  }

  json["Targets"] = targArray;
}

void TargetsCtrl::addTarget()
{
  Trajectories trajs = trajs_ctrl->getTrajs();

  if ((trajs_ctrl == NULL) || (trajs.empty())) {
    QMessageBox::information(0, "Добавление цели", "Нет ни одной заданной траектории.");
    return;
  }

  TargetDialog* pDialog = new TargetDialog(trajs.size());
  if (pDialog->exec() == QDialog::Accepted) {
    disconnect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
               this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

    bool ok1, ok2;
    unsigned traj_idx = pDialog->traj_num() - 1;
    double vel = pDialog->velocity().toDouble(&ok1);
    double acc = pDialog->accel().toDouble(&ok2);

    if (!ok1 || !ok2) {
      QMessageBox::information(0, "Добавление цели", "Неверно введены данные.");
      return;
    }

    Trajectories trajs = trajs_ctrl->getTrajs();
    if (trajs.size() <= traj_idx) return;

    Target targ(trajs[traj_idx]);
    targ.setVel(vel);
    targ.setAcc(acc);

    addTarg(targ);

    emit eventUpdate();

    connect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

    QMessageBox::information(0, "Добавление цели", "Цель успешно добавлена!");
  }
  delete pDialog;
}

void TargetsCtrl::addTarg(Target targ)
{
  targets.push_back(targ);

  QModelIndex root = tree_view.rootIndex();
  QModelIndex index_targ, index_prm;;

  unsigned targ_num = targs_model->rowCount(root);
  if (!targs_model->insertRow(targ_num, root))
    return;

  index_targ = targs_model->index(targ_num, 0, root);
  targs_model->setData(index_targ, "Target " + QString::number(targ_num + 1), Qt::EditRole);

  QPointF c = targ.getCoords();
  cout << "addTarget: x,y: " << c.x() << " " << c.y() << endl;

  targs_model->insertRow(0, index_targ);
  index_prm = targs_model->index(0, 0, index_targ);
  targs_model->setData(index_prm, "Traj idx", Qt::EditRole);
  index_prm = targs_model->index(0, 1, index_targ);
  targs_model->setData(index_prm, targ.getTrajID(), Qt::EditRole);

  targs_model->insertRow(1, index_targ);
  index_prm = targs_model->index(1, 0, index_targ);
  targs_model->setData(index_prm, "Vel", Qt::EditRole);
  index_prm = targs_model->index(1, 1, index_targ);
  targs_model->setData(index_prm, targ.getVel(), Qt::EditRole);

  targs_model->insertRow(2, index_targ);
  index_prm = targs_model->index(2, 0, index_targ);
  targs_model->setData(index_prm, "Acc", Qt::EditRole);
  index_prm = targs_model->index(2, 1, index_targ);
  targs_model->setData(index_prm, targ.getAcc(), Qt::EditRole);

  map_viewer->updateTargets(&targets);
}


void TargetsCtrl::deleteTarget()
{
  QModelIndex index = tree_view.selectionModel()->currentIndex();
  int idx = index.parent().isValid() ? index.parent().row() : index.row();
  if (idx < 0) return;

  cout << "TrajsCtrl::deleteTraj(), idx = " << idx << endl;

  if (idx < targets.size()) {
    disconnect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
               this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

    Targets::iterator it = targets.begin();
    for (unsigned i = 0; i < idx; ++i) it++;
    targets.erase(it);

    QModelIndex root = tree_view.rootIndex();
    targs_model->removeRow(idx, root);
    unsigned targ_num = targs_model->rowCount(root);
    for (unsigned i = 0; i < targ_num; ++i) {
      QModelIndex index = targs_model->index(i, 0, root);
      targs_model->setData(index, "Target " + QString::number(i + 1), Qt::EditRole);
    }
    map_viewer->updateTargets(&targets);

    connect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

    QMessageBox::information(0, "Удаление цели",
                      "Цель " + QString::number(idx+1) + " успешно удалена!");
  }

  emit eventUpdate();
}

void TargetsCtrl::deleteAllTargets()
{
  QModelIndex root = tree_view.rootIndex();
  unsigned targ_num = targs_model->rowCount(root);
  targs_model->removeRows(0, targ_num, root);

  targets.clear();

  map_viewer->updateTargets(&targets);
}

void TargetsCtrl::onDataChanged(QModelIndex tl, QModelIndex, QVector<int>)
{
  QModelIndex index_targ = tl.parent();

  int i_targ = index_targ.row();

  cout << __PRETTY_FUNCTION__ << ": idx = " << i_targ << endl;
  cout << "row() = " << tl.row() << endl;

  Target &t = targets[i_targ];

  if (tl.row() == 1) t.setVel(tl.data().toDouble());
  if (tl.row() == 2) t.setAcc(tl.data().toDouble());

 // trajs[i_t]->setPoints(pv);

  map_viewer->updateTargets(&targets);
}

void TargetsCtrl::onTargetClicked(int idx)
{
  QModelIndex index, root;
  root = tree_view.rootIndex();
  index = targs_model->index(idx, 0, root);
  cout << __PRETTY_FUNCTION__ << ", idx = " << idx << endl;

  tree_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

void TargetsCtrl::onTargetSelected(QModelIndex index)
{
  QModelIndex index_main = index;
  while(index_main.parent().isValid()) index_main = index_main.parent();
  int idx = index_main.row();

  map_viewer->selectTarget(idx);
}

void TargetsCtrl::setMapViewer(Mapviewer * mv)
{
  map_viewer = mv;

  connect(map_viewer, SIGNAL(targetClicked(int)), this, SLOT(onTargetClicked(int)));
  //connect(this, SIGNAL(targetSelected(int)), map_viewer, SLOT(selectTarget(int)));
}
