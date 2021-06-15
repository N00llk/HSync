#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>
#include <QMessageBox>
#include <QElapsedTimer>
#include <LibFCL_Types.h>

namespace Ui {
class SyncPage;
}

class SyncPage : public QWidget
{
    Q_OBJECT

public:
    explicit SyncPage(QWidget *parent = nullptr);
    ~SyncPage();

    QString *getLocalFilter(void);

private slots:

    void on_lineEditPageName_textChanged(const QString &arg1);

    void on_pushButtonGood_clicked();

    void on_pushButtonBad_clicked();

public slots:

    void on_pushButtonSync_clicked();

    void on_progressChanged(int value);

    void on_addFilePair(QString file1, QString file2, int result);

private:
    Ui::SyncPage *ui;
    QElapsedTimer timer;
    QString localFilter;

private:
    void resetTable(void);
    void setRowColor(int row, int result);

    void lockInterface(void);
    void unlockInterface(void);

signals:
    void pageNameChanged(QString newName);
    void syncStarted(QString source, QString dest);
};

