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
    Trajectories &trajs = trajs_ctrl->getTrajs();
    Target t(trajs.at(targObject["Traj_ID"].toInt()));

    // double start_time = targObject["Start"].toDouble();
    // t.setDelay(start_time);
    t.setIsActive(true);

    addTarg(t, targObject);
  }

  map_viewer->updateTargets(&targets);

  connect(targs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  QMessageBox::information(0, "Добавление целей", "Цели успешно добавлены!");
}

void TargetsCtrl::saveJSON(QJsonObject &json)
{
  QModelIndex root = tree_view.rootIndex();
  unsigned targ_num = targs_model->rowCount(root);
  QJsonArray targArray;

  for (int i_targ = 0; i_targ < targ_num; ++i_targ) {
    Target &targ = targets[i_targ];
    QJsonObject targObject;

    targObject.insert("Traj_ID", targ.getTrajID());

    double d1(0), d2(0), vel(0), acc(0);
    QModelIndex index_targ = targs_model->index(i_targ, 0, root);

    /// -1, потому что первая строка - Traj ID
    unsigned lines_num = targs_model->rowCount(index_targ) - 1;
    QJsonArray lineArray;

    for (int i = 0; i < lines_num; ++i) {
      QJsonObject lineObject;
      QModelIndex index_line = targs_model->index(i+1, 0, index_targ);
      d1  = targs_model->index(0, 1, index_line).data().toDouble();
      vel = targs_model->index(1, 1, index_line).data().toDouble();
      acc = targs_model->index(2, 1, index_line).data().toDouble();
      d2  = targs_model->index(3, 1, index_line).data().toDouble();

      lineObject.insert("Delay_1", d1);
      lineObject.insert("Vel",    vel);
      lineObject.insert("Acc",    acc);
      lineObject.insert("Delay_2", d2);
      lineArray.append(lineObject);
    }
    targObject.insert("Lines", lineArray);
    targArray.append(targObject);
  }
  json["Targets"] = targArray;
}

void TargetsCtrl::addTarget()
{
  Trajectories &trajs = trajs_ctrl->getTrajs();

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

/*
    //double vel = pDialog->velocity().toDouble(&ok1);
    //double acc = pDialog->accel().toDouble(&ok2);

    if (!ok1 || !ok2) {
      QMessageBox::information(0, "Добавление цели", "Неверно введены данные.");
      return;
    }
*/

    Trajectories trajs = trajs_ctrl->getTrajs();
    if (trajs.size() <= traj_idx) return;

    Target targ(trajs[traj_idx]);
    //targ.setVel(vel);
    //targ.setAcc(acc);

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
  addTarg(targ, QJsonObject());
}

void TargetsCtrl::addTarg(Target targ, QJsonObject json)
{
  targets.push_back(targ);

  QModelIndex root = tree_view.rootIndex();
  unsigned targ_num = targs_model->rowCount(root);
  if (!targs_model->insertRow(targ_num, root))
    return;

  QModelIndex index_targ = targs_model->index(targ_num, 0, root);
  targs_model->setData(index_targ, "Target " + QString::number(targ_num + 1), Qt::EditRole);
  targs_model->addPairKeyValue("Traj idx", targ.getTrajID() + 1, index_targ);
  QJsonArray lineArray = json["Lines"].toArray();

  int lines_num = targ.getPointsNum() - 1;
  for (int i = 0; i < lines_num; ++i) {
    QModelIndex index_line = targs_model->addPairKeyValue("Line " + QString::number(i+1) + "-" + QString::number(i+2), "", index_targ);

    double d1(0), v(1), a(0), d2(0);
    if (!json.empty()) {
      QJsonObject lineObject = lineArray[i].toObject();
      d1 = lineObject["Delay_1"].toInt();
      v  = lineObject["Vel"].toInt();
      a  = lineObject["Acc"].toInt();
      d2 = lineObject["Delay_2"].toInt();
    }

    targs_model->addPairKeyValue("Delay in " + QString::number(i+1),  d1,  index_line);
    targs_model->addPairKeyValue("Vel",                               v,  index_line);
    targs_model->addPairKeyValue("Acc",                               a,  index_line);
    targs_model->addPairKeyValue("Delay in " + QString::number(i+2),  d2,  index_line);
  }

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

void TargetsCtrl::createUpdaters()
{
  QModelIndex root = tree_view.rootIndex();
  Trajectories &trajs = trajs_ctrl->getTrajs();
  unsigned targ_num = targs_model->rowCount(root);

  for (int i_targ = 0; i_targ < targ_num; ++i_targ) {
    Target &targ = targets[i_targ];

    if (targ.getTrajID() >= trajs.size()) continue;

    LinearTargUpdater *upd = new LinearTargUpdater(0,0, QPointF(),QPointF(), 0,0);
    QModelIndex index_targ = targs_model->index(i_targ, 0, root);

    BaseTrajectory *traj = trajs[targ.getTrajID()];
    PointsVector pv = traj->getPoints();

    QPointF first, last;
    double beg(0), dur(0),
           delay_f(0), delay_l(0),
           vel(0), acc(0);

    /// -1, потому что первая строка - Traj ID
    unsigned lines_num = targs_model->rowCount(index_targ) - 1;

    if (lines_num != pv.size() - 1) {
      cout << "Несоответствие между маршрутом и отрезками: "
           << lines_num << " отрезков, " << pv.size() << " точек." << endl;
      continue;
    }

    for (int i = 0; i < lines_num; ++i) {
      first = pv[i];
      last  = pv[i+1];

      QModelIndex index_line = targs_model->index(i+1, 0, index_targ);
      delay_f = targs_model->index(0, 1, index_line).data().toDouble();
      vel     = targs_model->index(1, 1, index_line).data().toDouble() * 0.001;
      acc     = targs_model->index(2, 1, index_line).data().toDouble() * 0.001;
      delay_l = targs_model->index(3, 1, index_line).data().toDouble();

/*
      cout << __PRETTY_FUNCTION__ << " Model Data: " << targs_model->rowCount(index_line) << " rows: " << delay_f << " "
                                  << vel << " " << acc << " " << delay_l << endl;
*/

      if (!TrajTools::findDurationWithCheck(dur, vel,acc, first,last)) {
        cout << "Конечная точка отрезка недостижима при заданных параметрах" << endl;
        break;
      }
      // Пауза в начале отрезка
      if (delay_f > 10e-6) {
        upd->add(new LinearTargUpdater(beg,delay_f, first,QPointF(), 0,0));
        beg += delay_f;
      }
      // Движение на отрезке
      upd->add(new LinearTargUpdater(beg,dur, first,last-first, vel,acc));
      beg += dur;
      // Пауза в конце отрезка
      if (delay_l > 10e-6) {
        upd->add(new LinearTargUpdater(beg,delay_l, last,QPointF(), 0,0));
        beg += delay_l;
      }
      // cout << __PRETTY_FUNCTION__ << "beg, dur: " << beg << ", " << dur << endl;
    }
    targ.setUpdater(upd);
  }
}

void TargetsCtrl::onDataChanged(QModelIndex tl, QModelIndex, QVector<int>)
{
  QModelIndex index_targ = tl.parent();

  int i_targ = index_targ.row();

  cout << __PRETTY_FUNCTION__ << ": idx = " << i_targ << endl;
  cout << "row() = " << tl.row() << endl;

  Target &t = targets[i_targ];

  // if (tl.row() == 1) t.setVel(tl.data().toDouble());
  // if (tl.row() == 2) t.setAcc(tl.data().toDouble());

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
  // connect(this, SIGNAL(targetSelected(int)), map_viewer, SLOT(selectTarget(int)));
}

void TargetsCtrl::setLoopAll(int state)
{
  bool looped = (state > 0);
  for (Targets::iterator it = targets.begin(); it != targets.end(); it++) {
    it -> setIsLooped(looped);
  }
}
