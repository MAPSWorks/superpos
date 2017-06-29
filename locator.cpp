#include "locator.h"

#include <iostream>

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/GeodesicLine.hpp>

using namespace std;
using namespace GeographicLib;

struct LocCoordPoint {
  unsigned discr;
  double   phi;
};

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

  DATA_PACKAGE_AD d = parser.getData();
  data.push_back(d);

  for (unsigned n = 1; n < DATA_NUM_ONE_ROUND * getRoundsNum(); ++n)
    data.push_back(parser.getData());

  it_data = data.begin();
}

void Locator::writeToFile(Targets& targets)
{ 
  std::vector<LocCoordPoint> cps;
  Geocentric earth(Constants::WGS84_a(), Constants::WGS84_f());
  double lat0 = getCenter().y(),
         lon0 = getCenter().x(),
         h0   = 0;
  double x, y, z;
  LocalCartesian proj(lat0, lon0, h0, earth);

  for (Targets::iterator it = targets.begin(); it != targets.end(); ++it) {
    double lat = it->getCoords().y(),
           lon = it->getCoords().x(),
           h   = 0;

    proj.Forward(lat, lon, h, x, y, z);

    LocCoordPoint p;
    p.discr = sqrt(x*x + y*y) / METERS_IN_DISCR;
    p.phi = 180.0 / 3.14 * atan(-y / x);

    if (x < 0) p.phi += 180.0;
    if ((x > 0) && (y > 0)) p.phi += 360.0;

    cps.push_back(p);
  }

  double current_line_pos;

  do {
    current_line_pos = ((it_data->data.line_pos.pos + getLinePos0()) % MAX_LINE_POS);

    DATA_PACKAGE_AD d = *it_data;

    for (std::vector<LocCoordPoint>::iterator it = cps.begin(); it != cps.end(); ++it) {

      if ((fabs(current_line_pos * POS_TO_GRAD - it->phi) < 1000.0 / it->discr)
          && (it->discr < DISCR_NUM) && (it->discr > 10.0))
      {
        for (int i = -10; i < 11; ++i)
          d.data.out_data.spectr[it->discr + i] += 100000;
      }
    }

    out_file.write((char*)&d, sizeof(DATA_PACKAGE_AD));

    it_data++;
    if (it_data == data.end())
      it_data = data.begin();
  }
  while(fabs(current_line_pos * POS_TO_GRAD - getPhi()) > 3);
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
