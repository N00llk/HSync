#pragma once

#include <QMainWindow>
#include "SyncPage.h"
#include "FilterDialog.h"
#include <DIRSync.h>
#include <QtConcurrent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateActiveTabName(QString newName);


private slots:
    void on_createPage_triggered();

    void on_removePage_triggered();

    void on_syncStart(QString source, QString dest);

    void on_syncActivePage_triggered();

    void on_syncAllPages_triggered();

    void on_setupFilters_triggered();

    void on_saveTabs_triggered();

    void on_loadTabs_triggered();

    void on_cleanTabs_triggered();

    void on_about_triggered();

private:
    Ui::MainWindow *ui;
    DIRSync _synchronizer;
    QString globalFilter;
};
