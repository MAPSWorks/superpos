#include "widget.h"
#include "drawer.h"

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Widget wgt;
  wgt.setGeometry(20,20,1000,730);

  wgt.show();

  return a.exec();
}
