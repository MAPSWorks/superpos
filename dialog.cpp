#include <QtWidgets>

#include "dialog.h"

// ----------------------------------------------------------------------
LocatorDialog::LocatorDialog(QWidget* pwgt/*= 0*/)
     : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{   
    lat = new QLineEdit;
    lon  = new QLineEdit;

    QLabel* l_lat = new QLabel("&Широта");
    QLabel* l_lon = new QLabel("&Долгота");

    l_lat->setBuddy(lat);
    l_lon->setBuddy(lon);

    QPushButton* pcmdOk     = new QPushButton("&OK");
    QPushButton* pcmdCancel = new QPushButton("&Отмена");

    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(l_lat, 0, 0);
    ptopLayout->addWidget(l_lon, 1, 0);
    ptopLayout->addWidget(lat, 0, 1);
    ptopLayout->addWidget(lon, 1, 1);
    ptopLayout->addWidget(pcmdOk, 2,0);
    ptopLayout->addWidget(pcmdCancel, 2, 1);
    setLayout(ptopLayout);

    fname = QFileDialog::getOpenFileName(0, "Выберите файл с данными", "", "*.b");
}

QString LocatorDialog::latitude() const
{
    return lat->text();
}

QString LocatorDialog::longitude() const
{
    return lon->text();
}

QString LocatorDialog::filename() const
{
    return fname;
}

TargetDialog::TargetDialog(unsigned N, QWidget* pwgt/*= 0*/)
     : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    traj = new QSpinBox;
    vel = new QLineEdit;
    acc  = new QLineEdit;

    traj->setMaximum(N);

    QLabel* l_traj = new QLabel("&Траектория");
    QLabel* l_vel = new QLabel("&Скорость");
    QLabel* l_acc = new QLabel("&Ускорение");

    l_traj->setBuddy(traj);
    l_vel->setBuddy(vel);
    l_acc->setBuddy(acc);

    QPushButton* pcmdOk     = new QPushButton("&OK");
    QPushButton* pcmdCancel = new QPushButton("&Отмена");

    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(l_traj, 0, 0);
    ptopLayout->addWidget(l_vel, 1, 0);
    ptopLayout->addWidget(l_acc, 2, 0);
    ptopLayout->addWidget(traj, 0, 1);
    ptopLayout->addWidget(vel, 1, 1);
    ptopLayout->addWidget(acc, 2, 1);
    ptopLayout->addWidget(pcmdOk, 3,0);
    ptopLayout->addWidget(pcmdCancel, 3, 1);
    setLayout(ptopLayout);
}

unsigned TargetDialog::traj_num() const
{
  return traj->value();
}

QString TargetDialog::velocity() const
{
  return vel->text();
}

QString TargetDialog::accel() const
{
  return acc->text();
}
