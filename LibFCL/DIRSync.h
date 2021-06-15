#pragma once

#include <QString>
#include <QPair>
#include <QDirIterator>
#include <QDebug>
#include <QObject>
#include <QDir>
#include "LibFCL.h"

class DIRSync: public QObject
{
    Q_OBJECT
public:
    DIRSync();
    void sync(QString source, QString dest);
    void sync2(QString baseSource, QString baseDest, QString curSource, QString curDest, int level = 0);
    void reset(void);

private:
    void moveFile(QString baseFrom, QString baseTo, QString relFile);
    void copyFile(QString baseFrom, QString baseTo, QString relFile);
    void removeFilePair(QVector<QString> &files1, QVector<QString> &files2, QVector<QVector<int>> &result, int first, int second);

    void moveToDNDC(QString src, QString dst);
    void removeColumn(QVector<QVector<int>> &data, qint64 column);
    void cleanUp(QVector<QVector<int>> &data);

signals:
    void updateProgressBar(int value);
    void addNewFilePair(QString file1, QString file2, int result);
};

