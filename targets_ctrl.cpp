#include "targets_ctrl.h"
#include "dialog.h"

#include <QGridLayout>
#include <QJsonObject>
#include <QJsonArray>

TargetsCtrl::TargetsCtrl(QWidget *parent) : QWidget(parent)
{
  QStringList headers;
  headers << tr("lat") << tr("lon") << tr("delay");
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
  QJsonArray targArray = json["Targets"].toArray();

  cout << "There are " << targArray.size() << " targets." << endl;
}

void TargetsCtrl::saveJSON(QJsonObject &json)
{
  QJsonArray targArray;

  int i(0);
  for (Targets::iterator it = targets.begin(); it != targets.end(); it++) {
    QJsonObject targObject;

    targObject.insert("traj", it->getTrajID());
    targObject.insert("vel",  it->getVel());
    targObject.insert("acc",  it->getAcc());

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
    bool ok1, ok2;
    unsigned traj_num = pDialog->traj_num() - 1;
    double vel = pDialog->velocity().toDouble(&ok1);
    double acc = pDialog->accel().toDouble(&ok2);

    if (!ok1 || !ok2) {
      QMessageBox::information(0, "Добавление цели", "Неверно введены данные.");
      return;
    }

    Trajectories trajs = trajs_ctrl->getTrajs();
    if (trajs.size() <= traj_num) return;

    Target targ(trajs[traj_num]);
    targ.setVel(vel);
    targ.setAcc(acc);
    targets.push_back(targ);

    QModelIndex root = tree_view.rootIndex();
    QModelIndex index, child;

    unsigned targ_num = targs_model->rowCount(root);
    if (!targs_model->insertRow(targ_num, root))
      return;

    index = targs_model->index(targ_num, 0, root);
    targs_model->setData(index, "Target " + QString::number(targ_num + 1), Qt::EditRole);
    targs_model->insertRow(0, index);

    QPointF c = targ.getCoords();
    cout << "addTarget: x,y: " << c.x() << " " << c.y() << endl;

    child = targs_model->index(0, 0, index);
    targs_model->setData(child, QVariant(c.x()), Qt::EditRole);
    child = targs_model->index(0, 1, index);
    targs_model->setData(child, QVariant(c.y()), Qt::EditRole);

    map_viewer->updateTargets(&targets);

    emit eventUpdate();

    QMessageBox::information(0, "Добавление цели", "Цель успешно добавлена!");
  }
  delete pDialog;
}

void TargetsCtrl::deleteTarget()
{
  QModelIndex index = tree_view.selectionModel()->currentIndex();
  int idx = index.parent().isValid() ? index.parent().row() : index.row();
  if (idx < 0) return;

  cout << "TrajsCtrl::deleteTraj(), idx = " << idx << endl;

  if (idx < targets.size()) {
    /*disconnect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
               this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
*/
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
    // map_viewer->deleteTraj(idx);

  /*  connect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
*/
    QMessageBox::information(0, "Удаление траектории",
                            "Траектория " + QString::number(idx+1)
                                          + " успешно удалена!");
  }

  emit eventUpdate();
}

void TargetsCtrl::setMapViewer(Mapviewer * mv)
{
  map_viewer = mv;

 /* connect(map_viewer, SIGNAL(trajClicked(int)), this, SLOT(onTrajClicked(int)));
  connect(this, SIGNAL(trajSelected(int)), map_viewer, SLOT(selectTraj(int)));
  */
}
