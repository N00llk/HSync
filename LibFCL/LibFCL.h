#pragma once

#include <QString>
#include <QByteArray>
#include <QFileInfo>
#include "HashStorage.h"
#include "LibFCL_Types.h"

class LibFCL
{
public:
    LibFCL();
    ~LibFCL();
    int compare(QString firstFilePath, QString secondFilePath);
    void reset(void);
private:
    HashGroup readHashBlock(QFile &file, const qint64 &offset);

    QString firstFile;
    QString secondFile;

    HashStorage _storage;
};
