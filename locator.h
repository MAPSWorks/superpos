#ifndef LOCATOR_H
#define LOCATOR_H

#include <QPainter>
#include <QPixmap>
#include <vector>

#include "parser.h"

class Locator
{
  public:
    Locator();

    void init(QPoint, double);
    void addBackground(const char*);
    void addTarget();
    void updatePixmap();

    const QPixmap& getPixmap() {return pixmap;}

  private:
    QPixmap pixmap;
    Parser parser;

    QPoint center; // Координаты центра
    double angle0, // Смещение азимута
           phi;    // Текущий азимут
};

#endif // LOCATOR_H
