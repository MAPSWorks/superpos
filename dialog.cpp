#include <QtWidgets>
#include "dialog.h"

// ----------------------------------------------------------------------
InputDialog::InputDialog(QWidget* pwgt/*= 0*/) 
     : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{   
    lat = new QLineEdit;
    lon  = new QLineEdit;

    QLabel* l_lat = new QLabel("&Широта");
    QLabel* l_lon = new QLabel("&Долгота");

    l_lat->setBuddy(lat);
    l_lon->setBuddy(lon);

    QPushButton* pcmdOk     = new QPushButton("&Ok");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");

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

QString InputDialog::latitude() const
{
    return lat->text();
}

QString InputDialog::longitude() const
{
    return lon->text();
}

QString InputDialog::filename() const
{
    return fname;
}
