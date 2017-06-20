#ifndef DIALOG_H
#define DIALOG_H

#pragma once

#include <QDialog>

class QLineEdit;
class QSpinBox;

class LocatorDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* lat;
    QLineEdit* lon;
    QString    fname;

public:
    LocatorDialog(QWidget* pwgt = 0);

    QString filename() const;
    QString latitude() const;
    QString longitude() const;
};

class TargetDialog : public QDialog {
    Q_OBJECT
private:
    QSpinBox*  traj;
    QLineEdit* vel;
    QLineEdit* acc;

public:
    TargetDialog(unsigned, QWidget* pwgt = 0);

    unsigned traj_num() const;
    QString velocity() const;
    QString accel() const;
};

#endif // DIALOG_H
