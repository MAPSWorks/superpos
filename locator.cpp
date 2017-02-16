#include "locator.h"

#include <iostream>

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/LocalCartesian.hpp>

using namespace std;
using namespace GeographicLib;

#define COORDS(h,m,s) (h+m/60+s/3600)

Locator::Locator()
{
  pixmap = QPixmap(2*DISCR_NUM, 2*DISCR_NUM);
  pixmap.fill(Qt::transparent);
}

void Locator::init(QPoint cnt, double ang0)
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

}

void Locator::updatePixmap()
{
  QPainter painter;

  painter.begin(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(pixmap.width()/2, pixmap.height()/2);

  painter.scale(METERS_IN_DISCR, METERS_IN_DISCR);

  for (DataCont::iterator it = data.begin(); it != data.end(); ++it) {

    painter.rotate(it->data.line_pos.pos * POS_TO_GRAD);

    int step = 1;
    for (int i = 0; i < DISCR_NUM; i++) {
      float x = it->data.out_data.spectr[i];
      int col = pow(1.0 * x, 0.8);

      if (col < 0)   col = 0;
      if (col > 255) col = 255;

      painter.setPen( QPen(QColor(col,col,col), 50, Qt::SolidLine) );

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
