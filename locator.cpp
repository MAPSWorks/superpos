#include "locator.h"

#include <iostream>

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/LocalCartesian.hpp>

using namespace std;
using namespace GeographicLib;

#define COORDS(h,m,s) (h+m/60+s/3600)

Locator::Locator()
{
  pixmap = QPixmap(2*DISCR_NUM*SCALE, 2*DISCR_NUM*SCALE);
  pixmap.fill(Qt::transparent);

  file.open("RLS_Data_New.b", ios_base::binary | ios_base::out);
}

Locator::~Locator()
{
  closeFile();
}

void Locator::init(QPointF cnt, double ang0)
{
  center = cnt;
  angle0 = ang0;

  cout << "init" << "\n";

  Geocentric earth(Constants::WGS84_a(), Constants::WGS84_f());
 // Alternatively: const Geocentric& earth = Geocentric::WGS84();
 const double lat0 = COORDS(56.0,  8.0, 41.0),
              lon0 = COORDS(34.0, 59.0, 23.0);
 LocalCartesian proj(lat0, lon0, 0, earth);

   // Sample forward calculation
   double lat = COORDS(56.0,  8.0, 49.83),
          lon = COORDS(34.0, 59.0, 44.07),
          h = 0; // Calais
   double x, y, z;
   proj.Forward(lat, lon, h, x, y, z);
   cout << x << " " << y << " " << z << "\n";
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

  it_data = data.begin();
}

void Locator::writeToFile(Targets& targets)
{
  QPointF targ = targets.front().getCoords() - getCenter();
  targ.setY(-targ.ry());
  unsigned dist_discr = sqrt(targ.rx() * targ.rx() + targ.ry() * targ.ry())
                        * 72223.822090 / METERS_IN_DISCR;
  double targ_phi = 180.0 / 3.14 * atan(targ.ry() / targ.rx());
  if (targ.rx() < 0) targ_phi += 180.0;

  cout << "local_crd: " << targ.rx() << " " << targ.ry() << endl;
  cout << "line angle = " << it_data->data.line_pos.pos * POS_TO_GRAD
       << ", discr = " << dist_discr
       << ", targ_phi = " << targ_phi
       << ", phi = " << phi << endl;

  while(fabs(it_data->data.line_pos.pos * POS_TO_GRAD - phi) > 1.0)
  {
    DATA_PACKAGE_AD d = *it_data;

    if ((fabs(it_data->data.line_pos.pos * POS_TO_GRAD + angle0 - targ_phi) < 3.0)
        && (dist_discr < 4095))
    {
      cout << targ_phi << " " << dist_discr << endl;

      d.data.out_data.spectr[dist_discr-5] = 100000;
      d.data.out_data.spectr[dist_discr-4] = 100000;
      d.data.out_data.spectr[dist_discr-3] = 100000;
      d.data.out_data.spectr[dist_discr-2] = 100000;
      d.data.out_data.spectr[dist_discr-1] = 100000;
      d.data.out_data.spectr[dist_discr]   = 100000;
      d.data.out_data.spectr[dist_discr+1] = 100000;
      d.data.out_data.spectr[dist_discr+2] = 100000;
      d.data.out_data.spectr[dist_discr+3] = 100000;
      d.data.out_data.spectr[dist_discr+4] = 100000;
      d.data.out_data.spectr[dist_discr+5] = 100000;

    /*  d.data.out_data.spectr[1000] = 100000;
      d.data.out_data.spectr[1001] = 100000;
      d.data.out_data.spectr[1002] = 100000;
      d.data.out_data.spectr[1003] = 100000;
      d.data.out_data.spectr[1004] = 100000;
      d.data.out_data.spectr[1005] = 100000;
      d.data.out_data.spectr[1006] = 100000;
      d.data.out_data.spectr[1007] = 100000;
      d.data.out_data.spectr[1008] = 100000;
      d.data.out_data.spectr[1009] = 100000;
      d.data.out_data.spectr[1010] = 100000;
      d.data.out_data.spectr[1011] = 100000; */

  }

    file.write((char*)&d, sizeof(DATA_PACKAGE_AD));

    it_data++;
    if (it_data == data.end()) {
      it_data = data.begin();
    }
  }
}

void Locator::updatePixmap()
{
  QPainter painter;

  painter.begin(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(pixmap.width()/2, pixmap.height()/2);

  painter.scale(METERS_IN_DISCR*SCALE, METERS_IN_DISCR*SCALE);

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
