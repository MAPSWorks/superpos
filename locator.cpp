#include "locator.h"

#include <iostream>

using namespace std;

Locator::Locator()
{
}

Locator::~Locator()
{
  closeFile();
}

void Locator::init(QPointF cnt, const char * filename, int a0)
{
  // Начальные установки
  center = cnt;
  setAngle0(a0);
  setOpacity(0.8);

  loc_wgt.setFilename(filename);
  loc_wgt.setCoords(cnt);

  // Инициализация Pixmap
  pixmap = QPixmap(2*DISCR_NUM*SCALE, 2*DISCR_NUM*SCALE);
  pixmap.fill(Qt::transparent);

  // Открываем файл для выходных данных
  out_file.open("RLS_Data_New.b", ios_base::binary | ios_base::out);

  // Загрузка данных из файла записи
  parser.openFile(filename);
  data.reserve(DATA_NUM_ONE_ROUND);
  DATA_PACKAGE_AD d = parser.getData();
  phi = d.data.line_pos.pos * POS_TO_GRAD;
  data.push_back(parser.getData());
  for (unsigned n = 1; n < DATA_NUM_ONE_ROUND * getRoundsNum(); ++n)
    data.push_back(parser.getData());
  it_data = data.begin();

  cout << "Locator Init" << endl;
}

void Locator::writeToFile(Targets& targets)
{
  QPointF targ = targets.front().getCoords() - getCenter();
  targ.setY(-targ.ry());
  unsigned dist_discr = sqrt(targ.rx() * targ.rx() + targ.ry() * targ.ry())
                        * 72223.822090 / METERS_IN_DISCR;
  double targ_phi = 180.0 / 3.14 * atan(targ.ry() / targ.rx());
  if (targ.rx() < 0) targ_phi += 180.0;

#if 0
  cout << "line angle = " << it_data->data.line_pos.pos * POS_TO_GRAD
       << ", discr = " << dist_discr
       << ", targ_phi = " << targ_phi
       << ", phi = " << phi << endl;
#endif

  while(fabs(it_data->data.line_pos.pos * POS_TO_GRAD - phi) > 1.0)
  {
    DATA_PACKAGE_AD d = *it_data;

    if ((fabs(it_data->data.line_pos.pos * POS_TO_GRAD + angle0 - targ_phi) < 1000 / dist_discr)
        && (dist_discr < 4090))
    {
      for (int i = -5; i < 6; ++i)
        d.data.out_data.spectr[dist_discr] = 100000;
    }
    out_file.write((char*)&d, sizeof(DATA_PACKAGE_AD));

    it_data++;
    if (it_data == data.end())
      it_data = data.begin();
  }
}

void Locator::updatePixmap()
{
  cout << "updatePixmap" << endl;

  QPainter painter;

  pixmap.fill(Qt::transparent);

  painter.begin(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(pixmap.width()/2, pixmap.height()/2);
  painter.scale(METERS_IN_DISCR*SCALE, METERS_IN_DISCR*SCALE);
  painter.setOpacity(getOpacity());

  cout << "data size = " << data.size() << ", opacity = " << getOpacity() << endl;

  for (DataCont::iterator it = data.begin(); it != data.end(); ++it) {
    painter.rotate(it->data.line_pos.pos * POS_TO_GRAD);
    int step = 1;

    for (int i = 0; i < DISCR_NUM; i++) {
      float x = it->data.out_data.spectr[i];

      int col = pow(1.0 * x, 0.8);
      if (col > 255) col = 255;
      QColor color(col, col, col);
      if (col < 2) color = Qt::transparent;

      painter.setPen( QPen(color, 20, Qt::SolidLine) );
      painter.drawLine(i*step,0, (i+1)*step,0);
    }
    painter.rotate(- it->data.line_pos.pos * POS_TO_GRAD);
  }

  painter.end();
}

double Locator::getNextPhi()
{
  phi += 360 * DELTA_T / TIME_ONE_ROUND;
  if ((angle0 + phi) >= 360)
    phi -= 360;
  return angle0 + phi;
}
