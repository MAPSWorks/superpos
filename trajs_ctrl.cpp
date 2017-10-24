#include "trajs_ctrl.h"
#include "dialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QJsonObject>
#include <QJsonArray>


TrajsCtrl::TrajsCtrl(QWidget *parent) :
  QWidget(parent)
{
  QStringList headers;
  headers << tr("Ключ") << tr("Значение");
  trajs_model = new TreeModel(headers, QString());

  trajs.clear();

  pbBeginAddTraj.setText("Задать точки");
  pbBeginAddTraj.setGeometry(0,0, 100, 20);
  pbEndAddTraj.setText("Добавить тр-ю");
  pbEndAddTraj.setGeometry(0,0, 100, 20);
  pbEndAddTraj.setEnabled(false);
  pbDelTraj.setText("Удалить тр-ю");
  pbDelTraj.setGeometry(0,0, 100, 20);
  connect(&pbBeginAddTraj, SIGNAL(released()), SLOT(beginAddTraj()));
  connect(&pbEndAddTraj,   SIGNAL(released()), SLOT(endAddTraj()));
  connect(&pbDelTraj,      SIGNAL(released()), SLOT(deleteTraj()));

  connect(&tree_view, SIGNAL(clicked(QModelIndex)), SLOT(onTrajSelected(QModelIndex)));
  connect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  tree_view.setModel(trajs_model);
  tree_view.setWindowTitle(QObject::tr("Simple Tree Model"));
  tree_view.setItemDelegate(new DoubleSpinBoxDelegate);

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->addWidget(&pbBeginAddTraj);
  hLayout->addWidget(&pbEndAddTraj);
  hLayout->addWidget(&pbDelTraj);
  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->addWidget(&tree_view);
  vLayout->addLayout(hLayout);
  setLayout(vLayout);
}

void TrajsCtrl::setMapViewer(Mapviewer * mv)
{
  map_viewer = mv;

  connect(map_viewer, SIGNAL(trajClicked(int)), this, SLOT(onTrajClicked(int)));
  connect(this, SIGNAL(trajSelected(int)), map_viewer, SLOT(selectTraj(int)));
}

void TrajsCtrl::loadJSON(const QJsonObject &json)
{
  disconnect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  deleteAllTrajs();

  QPen* pen = new QPen(QColor(0,255,0, 100), 3);
  QJsonArray trajArray = json["Trajs"].toArray();

  for (int i = 0; i < trajArray.size(); ++i) {
    linestring = new LineString(QList<Point*>(), QString(), pen);

    QJsonObject trajObject = trajArray[i].toObject();
    int id = trajObject["ID"].toInt();
    QJsonArray pointArray = trajObject["Points"].toArray();
    PointsVector pv;

    for (int i_p = 0; i_p < pointArray.size(); ++i_p) {
      QJsonObject pointObject = pointArray[i_p].toObject();
      QPointF p(pointObject["x"].toDouble(), pointObject["y"].toDouble());
      linestring->addPoint(new CirclePoint(p.x(), p.y(), 10, QString(), Point::Middle, pen));
      pv.push_back(p);
    }

    LinearTrajectory *t = new LinearTrajectory(pv);
    t->setID(id);
    addTraj(t);
    map_viewer->addTraj(linestring);
  }
  map_viewer->updateTrajLayer();
  QMessageBox::information(0, "Добавление траектории", "Траектории успешно добавлены!");

  connect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

void TrajsCtrl::saveJSON(QJsonObject &json) const
{
  QJsonArray trajArray;

  int i(0);
  for (Trajectories::const_iterator it = trajs.begin(); it != trajs.end(); it++) {
    QJsonArray pointArray;
    QJsonObject trajObject;
    PointsVector pv = (*it).second->getPoints();

    int j(0);
    for (PointsVector::iterator it_p = pv.begin(); it_p != pv.end(); ++it_p, ++j) {
      QJsonObject pointObject;

      pointObject.insert("x", it_p->x());
      pointObject.insert("y", it_p->y());

      pointArray.append(pointObject);
    }
    trajObject.insert("ID", (*it).second->getID());
    trajObject.insert("Points", pointArray);

    trajArray.append(trajObject);
  }

  json["Trajs"] = trajArray;
}

void TrajsCtrl::beginAddTraj()
{
  connect(map_viewer, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(addTrajPoint(const QMouseEvent*,QPointF)));

  disconnect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  QPen* linepen = new QPen(QColor(0, 255, 0, 100));
  linepen->setWidth(3);
  linestring = new LineString(QList<Point*>(), QString(), linepen);

  map_viewer->addTraj(linestring);

  pbBeginAddTraj.setEnabled(false);
  pbEndAddTraj.setEnabled(true);
}

void TrajsCtrl::endAddTraj()
{
  disconnect(map_viewer, SIGNAL(mouseEventCoordinate(const QMouseEvent*,QPointF)),
          this, SLOT(addTrajPoint(const QMouseEvent*,QPointF)));

  if (!points_vector.empty()) {
    LinearTrajectory *t = new LinearTrajectory(points_vector);

    QModelIndex root = tree_view.rootIndex();
    QModelIndex index, child;

    unsigned tr_num = trajs_model->rowCount(root);

    addTraj(t);

    QMessageBox::information(0, "Добавление траектории", "Траектория успешно добавлена!");
  }
  else {
    QMessageBox::information(0, "Добавление траектории", "Ошибка. Не задано ни одной точки.");
  }
  points_vector.clear();

  pbBeginAddTraj.setEnabled(true);
  pbEndAddTraj.setEnabled(false);

  connect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

void TrajsCtrl::addTraj(BaseTrajectory* t)
{
  static int s_id(0);
  t->setID(s_id);
  trajs.insert(std::pair<int, BaseTrajectory*>(s_id, t));
  s_id++;

  cout << "addTraj, id " << s_id << endl;

  QModelIndex root = tree_view.rootIndex();
  QModelIndex index_tr, index_p, index_xy;

  unsigned tr_num = trajs_model->rowCount(root);
  if (!trajs_model->insertRow(tr_num, root))
    return;

  linestring->setName(QString::number(tr_num + 1));

  index_tr = trajs_model->index(tr_num, 0, root);
  trajs_model->setData(index_tr, "Traj " + QString::number(tr_num + 1), Qt::EditRole);

  int i = 0;
  const PointsVector &pv = t->getPoints();
  for (PointsVector::const_iterator it = pv.begin(); it != pv.end(); ++it, ++i) {
    trajs_model->insertRow(i, index_tr);
    index_p = trajs_model->index(i, 0, index_tr);
    trajs_model->setData(index_p, "Point " + QString::number(i+1), Qt::EditRole);

    trajs_model->insertRow(0, index_p);
    trajs_model->insertRow(1, index_p);

    index_xy = trajs_model->index(0, 0, index_p);
    trajs_model->setData(index_xy, "x", Qt::EditRole);
    index_xy = trajs_model->index(1, 0, index_p);
    trajs_model->setData(index_xy, "y", Qt::EditRole);

    index_xy = trajs_model->index(0, 1, index_p);
    trajs_model->setData(index_xy, QVariant(it->x()), Qt::EditRole);
    index_xy = trajs_model->index(1, 1, index_p);
    trajs_model->setData(index_xy, QVariant(it->y()), Qt::EditRole);
  }
}

void TrajsCtrl::deleteTraj()
{
  QModelIndex index = tree_view.selectionModel()->currentIndex();
  QModelIndex index_main = index;
  while(index_main.parent().isValid()) index_main = index_main.parent();
  int idx = index_main.row();
  if (idx < 0) return;

  cout << "TrajsCtrl::deleteTraj(), trajs.size() = " << trajs.size()
       << ", delete traj idx = " << idx
       <<  endl;

  if (idx < trajs.size()) {
    disconnect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
               this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

    Trajectories::iterator it = trajs.begin();
    for (unsigned i = 0; i < idx; ++i) it++;
    trajs.erase(it);

    QModelIndex root = tree_view.rootIndex();
    trajs_model->removeRow(idx, root);
    unsigned tr_num = trajs_model->rowCount(root);
    for (unsigned i = 0; i < tr_num; ++i) {
      QModelIndex index = trajs_model->index(i, 0, root);
      trajs_model->setData(index, "Traj " + QString::number(i + 1), Qt::EditRole);
    }

    map_viewer->deleteTraj(idx);

    connect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

    QMessageBox::information(0, "Удаление траектории",
                            "Траектория " + QString::number(idx+1)
                                          + " успешно удалена!");

    emit eventUpdate();
  }
}

void TrajsCtrl::deleteAllTrajs() {
  QModelIndex root = tree_view.rootIndex();
  unsigned tr_num = trajs_model->rowCount(root);

  trajs_model->removeRows(0, tr_num, root);
  trajs.clear();
  map_viewer->deleteAllTrajs();
}

void TrajsCtrl::onDataChanged(QModelIndex tl, QModelIndex, QVector<int>)
{
  QModelIndex index_main = tl;
  while(index_main.parent().isValid()) index_main = index_main.parent();
  int idx = index_main.row();

  cout << "TrajsCtrl::onDataChanged: idx = " << idx
       << endl;

  PointsVector pv = trajs[idx]->getPoints();
  if ((tl.row() >= pv.size()) || (tl.column() >= 2)) return;

  QPointF &p = pv[tl.row()];
  if (tl.column() == 0) p.setX(tl.data().toDouble());
  if (tl.column() == 1) p.setY(tl.data().toDouble());

  trajs[idx]->setPoints(pv);

  map_viewer->updateTraj(idx);
  map_viewer->selectTraj(idx);
}

void TrajsCtrl::onTrajClicked(int idx)
{
  QModelIndex index, root;
  root = tree_view.rootIndex();
  index = trajs_model->index(idx, 0, root);

  tree_view.selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

void TrajsCtrl::onTrajSelected(QModelIndex index)
{
  QModelIndex index_main = index;
  while(index_main.parent().isValid()) index_main = index_main.parent();
  int idx = index_main.row();

  map_viewer->selectTraj(idx);
}

void TrajsCtrl::addTrajPoint(const QMouseEvent* e, QPointF p)
{
  if (e->type() == QEvent::MouseButtonPress) {
    cout << "Point " << p.x() << " " << p.y() << endl;

    points_vector.push_back(p);

    QPen* pen = new QPen(QColor(0,255,0, 100), 3);
    linestring->addPoint(new CirclePoint(p.x(), p.y(), 10, QString(), Point::Middle, pen));

    map_viewer->updateTrajLayer();
  }
}
