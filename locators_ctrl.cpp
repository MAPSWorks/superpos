#include "locators_ctrl.h"
#include "dialog.h"

#include <QGridLayout>

LocatorsCtrl::LocatorsCtrl()
{
  locators.clear();

  locators.push_back(Locator());
  locators.back().init(QPointF(COORDS(34.0, 59.0, 44.07), COORDS(56.0,  8.0, 49.83)),
             "/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/2_250316/RLS_1_fileRLS_FFT_001.b", 501); // 44 град

  updateTabWidget();

  pbAddLoc.setText("Добавить РЛС");
  pbAddLoc.setGeometry(0,0, 100, 20);
  pbDelLoc.setText("Удалить РЛС");
  pbDelLoc.setGeometry(0,0, 100, 20);

  connect(&pbAddLoc, SIGNAL(released()), SLOT(addLocator()));
  connect(&pbDelLoc, SIGNAL(released()), SLOT(deleteLocator()));

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->addWidget(&pbAddLoc);
  hLayout->addWidget(&pbDelLoc);

  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->addWidget(&tab_wgt);
  vLayout->addLayout(hLayout);

  setLayout(vLayout);
}

void LocatorsCtrl::addLocator()
{
  LocatorDialog* pInputDialog = new LocatorDialog;
  if (pInputDialog->exec() == QDialog::Accepted) {
    bool ok1, ok2;
    double lat = pInputDialog->latitude().toDouble(&ok1);
    double lon = pInputDialog->longitude().toDouble(&ok2);
    QString fname = pInputDialog->filename();

    if (!ok1 || !ok2) {
      QMessageBox::information(0, "Добавление РЛС", "Неверный формат координат.");
      return;
    }

    locators.push_back(Locator());
    locators.back().init(QPointF(lat, lon), fname.toStdString().c_str(), 0);

    updateTabWidget();
    emit eventUpdate();

    QMessageBox::information(0, "Добавление РЛС", "Локатор успешно добавлен!");
  }
  delete pInputDialog;
}

void LocatorsCtrl::deleteLocator()
{
  unsigned idx = tab_wgt.currentIndex();

  if (idx < locators.size()) {
    Locators::iterator it = locators.begin();
    for (unsigned i = 0; i < idx; ++i) it++;
    locators.erase(it);
    QMessageBox::information(0, "Удаление РЛС",
                            "Локатор " + QString::number(idx+1) + " успешно удален!");
  }

  updateTabWidget();
  emit eventUpdate();
}

QPointF LocatorsCtrl::getCenter()
{
  QPointF c;

  if (!locators.empty())
    c = locators.front().getCenter();

  return c;
}

void LocatorsCtrl::updateCommonParams(const CommonParams& p)
{
  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it) {
    it->setFirstDiscr(p.first_discr);
    it->setLastDiscr(p.last_discr);
    it->setMinAmpl(p.min_ampl);
    it->setColorInvert(p.is_color_invert);
  }
}

void LocatorsCtrl::updateTabWidget()
{
  int i = 0;
  int cur = tab_wgt.currentIndex();
  tab_wgt.clear();

  for (Locators::iterator it = locators.begin(); it != locators.end(); ++it, ++i) {
    tab_wgt.addTab(&it->getLocatorWidget(), "РЛС " + QString::number(i+1));
  }
  if (cur <= i)
    tab_wgt.setCurrentIndex(cur);
}
