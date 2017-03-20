#include "widget.h"
#include "drawer.h"
#include "mapviewer.h"

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Widget wgt;
  wgt.setGeometry(20,20,1000,700);

  Mapviewer mv(&wgt);
  mv.resize(380, 565);
  mv.show();

  wgt.show();

  return a.exec();
}
