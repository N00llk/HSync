#include "HashStorage.h"
#include <QDebug>

HashStorage::HashStorage()
{

}

void HashStorage::addFile(QString filePath)
{
    _storage[filePath];
}

void HashStorage::addHash(QString filePath, HashGroup hash)
{
    _storage[filePath].append(hash);
}

const QVector<HashGroup> &HashStorage::getHash(QString filePath)
{
    return _storage[filePath];
}

bool HashStorage::exist(QString filePath) const
{
    return _storage.find(filePath) != _storage.end();
}

qint64 HashStorage::getHashCount(const QString &filePath) const
{
    return _storage[filePath].size();
}

void HashStorage::reset()
{
    _storage.clear();
}
