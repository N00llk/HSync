#pragma once

#include <QHash>
#include <QByteArray>
#include <QVector>
#include "LibFCL_Types.h"

struct HashGroup
{
    QByteArray strong;
    QByteArray weak[BLOCK_COUNT];

    friend bool operator==(const HashGroup& left, const HashGroup& right)
    {
        for (int i = 0; i < BLOCK_COUNT; i++)
        {
            if (left.weak[i] != right.weak[i])
            {
                return false;
            }
        }

        if (left.strong != right.strong)
        {
            return false;
        }

        return true;
    }
};


class HashStorage
{

public:
    HashStorage();

    void addFile(QString filePath);

    void addHash(QString filePath, HashGroup hash);

    const QVector<HashGroup>& getHash(QString filePath);

    bool exist(QString filePath) const;

    qint64 getHashCount(const QString &filePath) const;

    void reset(void);

private:
    QHash <QString, QVector<HashGroup>> _storage;
};

