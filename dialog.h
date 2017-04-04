#ifndef DIALOG_H
#define DIALOG_H

#pragma once

#include <QDialog>

class QLineEdit;

class InputDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* lat;
    QLineEdit* lon;
    QString    fname;

public:
    InputDialog(QWidget* pwgt = 0);

    QString filename() const;
    QString latitude() const;
    QString longitude() const;
};

#endif // DIALOG_H
