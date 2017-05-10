#include "locator.h"

#include <iostream>

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/GeodesicLine.hpp>

using namespace std;
using namespace GeographicLib;

Locator::Locator()
{
}

Locator::~Locator()
{
  closeFile();
}

void Locator::init(QPointF cnt, const char * filename, int lp0)
{
  // Начальные установки
  center = cnt;
  setLinePos0(lp0);
  setOpacity(0.8);

  loc_wgt.setFilename(filename);
  loc_wgt.setCoords(cnt);

  // Инициализация Pixmap
  pixmap = QPixmap(2*DISCR_NUM*SCALE, 2*DISCR_NUM*SCALE);
  pixmap.fill(Qt::transparent);

  // Загрузка данных из файла записи
  parser.openFile(filename);
  data.reserve(DATA_NUM_ONE_ROUND);
  //DATA_PACKAGE_AD d = parser.getData();
  data.push_back(parser.getData());
  for (unsigned n = 1; n < DATA_NUM_ONE_ROUND * getRoundsNum(); ++n)
    data.push_back(parser.getData());
  it_data = data.begin();
}

void Locator::writeToFile(Targets& targets)
{
  //QPointF targ = targets.front().getCoords() - getCenter();

/* С геодезическими координатами
  Geodesic geod(Constants::WGS84_a(), Constants::WGS84_f());

  double lat = targets.front().getCoords().x(),
         lon = targets.front().getCoords().y();
  double lat0 = getCenter().x(),
         lon0 = getCenter().y();

  cout << "Locator: " << lat0 << ", " << lon0 << endl;
  cout << "Target: "  << lat  << ", " << lon  << endl;

  const GeodesicLine line = geod.InverseLine(lat0, lon0, lat, lon);

  double dist, azim1, azim2, red_l;

  geod.Inverse(lat0, lon0, lat, lon, dist, azim1, azim2, red_l);

  cout << "Distance = "   << line.Distance() << " " << dist
       << ", Azimuth = "  << line.Azimuth() << " " << azim1 << " " << azim2
       << ", Red length = "  << red_l << endl;

*/

  Geocentric earth(Constants::WGS84_a(), Constants::WGS84_f());

  double lat0 = getCenter().x(),
         lon0 = getCenter().y();
  LocalCartesian proj(lat0, lon0, 0, earth);

  double lat = targets.front().getCoords().x(),
         lon = targets.front().getCoords().y(),
         h = 0;

  double x, y, z;
  proj.Forward(lat, lon, h, y, x, z);
  // cout << x << " " << y << " " << z << "\n";

  unsigned dist_discr = sqrt(x*x + y*y) / METERS_IN_DISCR;

  double targ_phi = 180.0 / 3.14 * atan(-y / x);
  if (x < 0) targ_phi += 180.0;

  // cout << "Dist Discr = " << dist_discr << ", Azim = " << targ_phi << "\n";

  while(fabs(it_data->data.line_pos.pos * POS_TO_GRAD - getPhi()) > 3)
  {
/*
    cout << "Loc Az = " << it_data->data.line_pos.pos * POS_TO_GRAD
         << ", Phi = " << getPhi() << endl;
*/
    DATA_PACKAGE_AD d = *it_data;

    if ((fabs((it_data->data.line_pos.pos + getLinePos0()) * POS_TO_GRAD - targ_phi) < 1000.0 / dist_discr)
        && (dist_discr < DISCR_NUM) && (dist_discr > 10.0))
    {

      for (int i = -10; i < 11; ++i)
        d.data.out_data.spectr[dist_discr + i] = 1000000;

    }
    out_file.write((char*)&d, sizeof(DATA_PACKAGE_AD));

    it_data++;
    if (it_data == data.end())
      it_data = data.begin();
  }
}

void Locator::setOutFile(const char* name)
{
  if (out_file.is_open())
    out_file.close();
  out_file.open(name, ios_base::binary | ios_base::out);

}

void Locator::updatePixmap()
{
  QPainter painter;

  pixmap.fill(Qt::transparent);

  painter.begin(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(pixmap.width()/2, pixmap.height()/2);
  painter.scale(METERS_IN_DISCR*SCALE, METERS_IN_DISCR*SCALE);
  painter.setOpacity(getOpacity());

  for (DataCont::iterator it = data.begin(); it != data.end(); ++it) {
    double angle = (it->data.line_pos.pos + getLinePos0()) * POS_TO_GRAD;
    painter.rotate(angle);

    unsigned step = 1;
    for (unsigned i = first_discr; i < last_discr; i++) {
      float x = it->data.out_data.spectr[i];

      if (i == 1000)
        x = 100000;

      QColor color;

      if (x < min_ampl)
        color = Qt::transparent;
      else {
        int col = pow(1.0 * x, 0.8);
        if (col > 255) col = 255;
        if (is_color_invert) col = 255 - col;
        color = QColor(col, col, col);
      }

      painter.setPen( QPen(color, 20, Qt::SolidLine) );
      painter.drawLine(i*step,0, (i+1)*step,0);
    }
    painter.rotate(-angle);
  }

  painter.end();
}

void Locator::start()
{
  startTime = chrono::system_clock::now();
}


double Locator::getPhi()
{
  chrono::time_point<chrono::system_clock> cur = std::chrono::system_clock::now();

  double elaps_ns = chrono::duration_cast<std::chrono::nanoseconds>
                           (cur-startTime).count();

  double t = elaps_ns * 1e-9;

  // TODO: Сделать нормально!
  return (int)(getAngle0() + 360.0 * getSpeed() * t) % 360;
}
