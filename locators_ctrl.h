#ifndef LOCATORSCTRL_H
#define LOCATORSCTRL_H

#include <QObject>
#include <QTabWidget>
#include <QPointF>
#include <QMessageBox>

#include "locator.h"
#include "params_groupbox.h"

class LocatorsCtrl: public QObject
{
  Q_OBJECT

  public:
    LocatorsCtrl();

    Locators& getLocators() { return locators; }
    QTabWidget& getTabWidget()  { return tab_wgt; }

    QPointF getCenter();
    void initTabWidget(QWidget*, const QRect&);
    void updateCommonParams(const CommonParams& p);

  signals:
    void eventUpdate();

  public slots:
    void addLocator();
    void deleteLocators();
    void updateTabWidget();

  private:
    Locators locators;
    QTabWidget tab_wgt;
};

#endif // LOCATORSCTRL_H
