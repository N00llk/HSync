#pragma once

#include <QDialog>
#include <QString>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
     FilterDialog(QWidget *parent, QString *globalFilter, QString *localFilter);
    ~FilterDialog();

private:
    Ui::FilterDialog *ui;
};

