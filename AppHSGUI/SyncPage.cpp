#include "SyncPage.h"
#include "ui_SyncPage.h"

SyncPage::SyncPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SyncPage)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    resetTable();
}

SyncPage::~SyncPage()
{
    delete ui;
}

QString *SyncPage::getLocalFilter()
{
    return &localFilter;
}

void SyncPage::on_lineEditPageName_textChanged(const QString &arg1)
{
    emit pageNameChanged(arg1);
}

void SyncPage::on_pushButtonGood_clicked()
{
    QString tmp = QFileDialog::getExistingDirectory(this, "Ведущий каталог", QDir::homePath());

    if (tmp != "")
    {
        ui->lineEditGood->setText(tmp);
    }
}

void SyncPage::on_pushButtonBad_clicked()
{
    QString tmp = QFileDialog::getExistingDirectory(this, "Ведомый каталог", QDir::homePath());

    if (tmp != "")
    {
        ui->lineEditBad->setText(tmp);
    }
}

void SyncPage::on_pushButtonSync_clicked()
{
    QDir check;

    if (!check.exists(ui->lineEditGood->text()))
    {
        QMessageBox::warning(this, "Внимание", "Не допустимый ведущий каталог.");
    }
    else if (!check.exists(ui->lineEditBad->text()))
    {
        QMessageBox::warning(this, "Внимание", "Не допустимый ведомый каталог.");
    }
    else
    {
        lockInterface();
        timer.start();
        resetTable();
        emit syncStarted(ui->lineEditGood->text(), ui->lineEditBad->text());
    }
}

void SyncPage::on_progressChanged(int value)
{
    if (value == 100)
    {
        unlockInterface();
        qDebug() << timer.nsecsElapsed();
    }
    ui->progressBar->show();
    ui->progressBar->setValue(value);
}

void SyncPage::on_addFilePair(QString file1, QString file2, int result)
{
    int rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(file1));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(file2));
    setRowColor(rowCount, result);
}

void SyncPage::resetTable()
{
    ui->tableWidget->setRowCount(0);
}

void SyncPage::setRowColor(int row, int result)
{
    ui->tableWidget->item(row, 0)->setTextColor(QColor::fromRgb(0, 0, 0));
    ui->tableWidget->item(row, 1)->setTextColor(QColor::fromRgb(0, 0, 0));
    ui->tableWidget->item(row, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableWidget->item(row, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    switch (result)
    {
    case FILE_HAS_BEEN_COPIED:
        ui->tableWidget->item(row, 0)->setBackgroundColor(QColor::fromRgb(0, 255, 0));
        ui->tableWidget->item(row, 1)->setBackgroundColor(QColor::fromRgb(0, 255, 0));
        break;
    case FILE_EXIST:
        break;
    case FILE_HAS_BEEN_MOVED:
        ui->tableWidget->item(row, 0)->setBackgroundColor(QColor::fromRgb(255, 0, 0));
        ui->tableWidget->item(row, 1)->setBackgroundColor(QColor::fromRgb(255, 0, 0));
        break;
    }
}

void SyncPage::lockInterface()
{
    ui->pushButtonBad->setDisabled(true);
    ui->pushButtonGood->setDisabled(true);
    ui->pushButtonSync->setDisabled(true);
    ui->pushButtonCheck->setDisabled(true);
    ui->lineEditBad->setDisabled(true);
    ui->lineEditGood->setDisabled(true);
    ui->lineEditPageName->setDisabled(true);
}

void SyncPage::unlockInterface()
{
    ui->pushButtonBad->setDisabled(false);
    ui->pushButtonGood->setDisabled(false);
    ui->pushButtonSync->setDisabled(false);
    ui->pushButtonCheck->setDisabled(false);
    ui->lineEditBad->setDisabled(false);
    ui->lineEditGood->setDisabled(false);
    ui->lineEditPageName->setDisabled(false);
}
