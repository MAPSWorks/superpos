#include "trajs_ctrl.h"
#include "dialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QListWidget>

TrajsCtrl::TrajsCtrl(QWidget *parent) :
  QWidget(parent)
{
  QStringList headers;
  headers << tr("lat") << tr("lon");
  trajs_model = new TreeModel(headers, QString());

  trajs.clear();

  updateWidget();

  pbBeginAddTraj.setText("Задать точки");
  pbBeginAddTraj.setGeometry(0,0, 100, 20);
  pbEndAddTraj.setText("Добавить тр-ю");
  pbEndAddTraj.setGeometry(0,0, 100, 20);
  pbDelTraj.setText("Удалить тр-ю");
  pbDelTraj.setGeometry(0,0, 100, 20);

  pbEndAddTraj.setEnabled(false);

  connect(&pbBeginAddTraj, SIGNAL(released()), SLOT(beginAddTraj()));
  connect(&pbEndAddTraj,   SIGNAL(released()), SLOT(endAddTraj()));
  connect(&pbDelTraj,      SIGNAL(released()), SLOT(deleteTraj()));

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->addWidget(&pbBeginAddTraj);
  hLayout->addWidget(&pbEndAddTraj);
  hLayout->addWidget(&pbDelTraj);

  connect(trajs_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
          this,        SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

  tree_view.setModel(trajs_model);
  tree_view.setWindowTitle(QObject::tr("Simple Tree Model"));
  tree_view.setItemDelegate(new DoubleSpinBoxDelegate);

  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->addWidget(&tree_view);
  vLayout->addLayout(hLayout);

  setLayout(vLayout);
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
    trajs.push_back(t);

    QModelIndex root = tree_view.rootIndex();
    QModelIndex index, child;

    unsigned tr_num = trajs_model->rowCount(root);

    if (!trajs_model->insertRow(tr_num, root))
      return;

    index = trajs_model->index(tr_num, 0, root);
    trajs_model->setData(index, "Traj " + QString::number(tr_num + 1), Qt::EditRole);

    int i = 0;
    for (PointsVector::iterator it = points_vector.begin();
                                it != points_vector.end(); ++it, ++i) {

        trajs_model->insertRow(i, index);

        cout << "endAddTraj: x,y: " << it->x() << " " << it->y() << endl;

        child = trajs_model->index(i, 0, index);
        trajs_model->setData(child, QVariant(it->x()), Qt::EditRole);
        child = trajs_model->index(i, 1, index);
        trajs_model->setData(child, QVariant(it->y()), Qt::EditRole);
    }

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

  updateWidget();
}

void TrajsCtrl::deleteTraj()
{
  QModelIndex index = tree_view.selectionModel()->currentIndex();

  unsigned idx;
  if (!index.parent().isValid())
    idx = index.row();
  else
    idx = index.parent().row();

  cout << "TrajsCtrl::deleteTraj(), idx = " << idx << endl;

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

    updateWidget();
    emit eventUpdate();
  }
}

void TrajsCtrl::onDataChanged(QModelIndex tl, QModelIndex, QVector<int>)
{
  cout << "Data Changed: " << tl.column() << ", " << tl.row() << endl;

  PointsVector pv = trajs[0]->getPoints();

  if ((tl.row() >= pv.size()) || (tl.column() >= 2)) return;

  QPointF &p = pv[tl.row()];
  if (tl.column() == 0) p.setX(tl.data().toDouble());
  if (tl.column() == 1) p.setY(tl.data().toDouble());

  trajs[0]->setPoints(pv);

  map_viewer->updateTraj(0);
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

void TrajsCtrl::updateWidget()
{
  int i = 0;
  int cur = tab_wgt.currentIndex();
  tab_wgt.clear();

  for (Trajectories::iterator it = trajs.begin(); it != trajs.end(); ++it, ++i) {
    PointsVector pv = (*it)->getPoints();
    QListWidget *lw = new QListWidget;

    for (PointsVector::iterator it_p = pv.begin(); it_p != pv.end(); ++it_p) {
      lw->addItem(QString::number(it_p->x()) + ", " + QString::number(it_p->y()));
    }

    tab_wgt.addTab(lw, "Тр." + QString::number(i+1));
  }

  if (cur <= i)
    tab_wgt.setCurrentIndex(cur);
}
