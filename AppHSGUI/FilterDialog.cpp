#include "FilterDialog.h"
#include "ui_FilterDialog.h"

FilterDialog::FilterDialog(QWidget *parent, QString *globalFilter, QString *localFilter) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    if (localFilter != nullptr)
    {
        ui->localFilter->setText(*localFilter);
    }
    ui->globalFilter->setText(*globalFilter);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}
