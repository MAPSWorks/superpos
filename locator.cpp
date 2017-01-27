#include "locator.h"

Locator::Locator()
{
  pixmap = QPixmap(400, 400);
  pixmap.fill(Qt::white);
}

void Locator::init(QPoint cnt, double ang0)
{
  center = cnt;
  angle0 = ang0;
}


void Locator::updatePixmap()
{
  QPainter painter;

  painter.begin(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(center);
  painter.scale(0.03, 0.03);

  for (int n = 0; n < 750; ++n) {

    DATA_PACKAGE_AD data = parser.getData();
    painter.rotate(360.0 * data.data.line_pos.pos / MAX_LINE_POS);

    int step = 1;
    for (int i = 0; i < DATA_LEN_SPECTR_4K; i++) {
      float x = data.data.out_data.spectr[i];
      int col = pow(0.9 * x, 0.8);

      if (col < 0)   col = 0;
      if (col > 255) col = 255;

      painter.setPen( QPen(QColor(col,col,col), 30, Qt::SolidLine) );

      painter.drawLine(i*step,i*step, (i+1)*step,(i+1)*step);
    }

    painter.rotate(- 360.0 * data.data.line_pos.pos / MAX_LINE_POS);
  }

  painter.end();
}
