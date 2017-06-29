#ifndef LOCATORSCTRL_H
#define LOCATORSCTRL_H

#include <QWidget>
#include <QTabWidget>
#include <QPointF>
#include <QMessageBox>
#include <QPushButton>

#include "locator.h"
#include "params_groupbox.h"

class LocatorsCtrl: public QWidget
{
  Q_OBJECT

  public:
    LocatorsCtrl();

    Locators& getLocators() { return locators; }

    QPointF getCenter();
    void updateCommonParams(const CommonParams& p);

  signals:
    void eventUpdate();

  public slots:
    void addLocator();
    void deleteLocator();
    void updateWidget();

  private:
    Locators locators;
    QTabWidget tab_wgt;

    QPushButton pbAddLoc,
                pbDelLoc;
};

#endif // LOCATORSCTRL_H
