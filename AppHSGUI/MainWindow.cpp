#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateActiveTabName(QString newName)
{
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), newName);
}

void MainWindow::on_createPage_triggered()
{
    SyncPage *tmp = new SyncPage(this);
    ui->tabWidget->addTab(tmp, "Новая страница");
    connect(tmp, &SyncPage::pageNameChanged, this, &MainWindow::updateActiveTabName);
    connect(tmp, &SyncPage::syncStarted, this, &MainWindow::on_syncStart);
    connect(&_synchronizer, &DIRSync::updateProgressBar, tmp, &SyncPage::on_progressChanged);
    connect(&_synchronizer, &DIRSync::addNewFilePair, tmp, &SyncPage::on_addFilePair);
}

void MainWindow::on_removePage_triggered()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void MainWindow::on_syncStart(QString source, QString dest)
{
    QFuture<void> future = QtConcurrent::run(&_synchronizer, &DIRSync::sync2, source, dest, source, dest, 0);
}

void MainWindow::on_syncActivePage_triggered()
{
    SyncPage* page = (SyncPage*)ui->tabWidget->currentWidget();
    if (page != nullptr)
    {
        page->on_pushButtonSync_clicked();
    }
    else
    {
        QMessageBox::warning(this, "Внимание", "Нет активной вкладки.");
    }
}

void MainWindow::on_syncAllPages_triggered()
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        ((SyncPage*)ui->tabWidget->widget(i))->on_pushButtonSync_clicked();
    }
}

void MainWindow::on_setupFilters_triggered()
{
    SyncPage* page = (SyncPage*)ui->tabWidget->currentWidget();
    QString* localFilter = nullptr;
    if (page != nullptr)
    {
        localFilter = page->getLocalFilter();
    }

    FilterDialog* dialog = new FilterDialog(this, &globalFilter, localFilter);
    dialog->exec();
    if (dialog->result())
    {
        qDebug() << "1";
    }
    else
    {
        qDebug() << "0";
    }

}

void MainWindow::on_saveTabs_triggered()
{

}

void MainWindow::on_loadTabs_triggered()
{

}

void MainWindow::on_cleanTabs_triggered()
{

}

void MainWindow::on_about_triggered()
{

}
