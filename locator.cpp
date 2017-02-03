#include "locator.h"

Locator::Locator()
{
  pixmap = QPixmap(400,400);
}

void Locator::init(QPoint cnt, double ang0)
{
  center = cnt;
  angle0 = ang0;
  deltaT = 0.1;
}

void Locator::addBackground(const char * filename)
{
  parser.openFile(filename);

  data.reserve(DATA_NUM_ONE_ROUND);

  DATA_PACKAGE_AD d = parser.getData();
  phi = d.data.line_pos.pos * POS_TO_GRAD;
  data.push_back(parser.getData());

  for (int n = 1; n < DATA_NUM_ONE_ROUND; ++n) {
    data.push_back(parser.getData());
  }

}

void Locator::updatePixmap()
{
  QPainter painter;

  painter.begin(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(200,200);
  painter.scale(0.1, 0.1);

  for (DataCont::iterator it = data.begin(); it != data.end(); ++it) {

    painter.rotate(it->data.line_pos.pos * POS_TO_GRAD);

    int step = 1;
    for (int i = 0; i < 1000/*DATA_LEN_SPECTR_4K*/; i++) {
      float x = it->data.out_data.spectr[i];
      int col = pow(0.9 * x, 0.8);

      if (col < 0)   col = 0;
      if (col > 255) col = 255;

      painter.setPen( QPen(QColor(col,col,col), 50, Qt::SolidLine) );

      painter.drawLine(i*step,i*step, (i+1)*step,(i+1)*step);
    }

    painter.rotate(- it->data.line_pos.pos * POS_TO_GRAD);
  }

  painter.end();
}

double Locator::getNextPhi()
{
  phi += 6.28 * deltaT / TIME_ONE_ROUND;
  if (phi > 6.28)
    phi -= 6.28;
  return phi;
}
