#include "LibFCL.h"

#include <QCryptographicHash>
#include <QDebug>

LibFCL::LibFCL()
{
}

LibFCL::~LibFCL()
{
}

int LibFCL::compare(QString firstFilePath, QString secondFilePath)
{
    QFileInfo firstFI(firstFilePath);
    QFileInfo secondFI(secondFilePath);

    if (!firstFI.exists())
        return FIRST_FILE_NOT_EXIST;

    if (!secondFI.exists())
        return SECOND_FILE_NOT_EXIST;

    if (firstFI.size() != secondFI.size())
    {
        if (firstFI.fileName() != secondFI.fileName())
        {
            return NAMES_DIFF_CONTENT_DIFF;
        }
        else
        {
            return NAMES_SAME_CONTENT_DIFF;
        }
    }

    QFile file1(firstFI.absoluteFilePath());
    QFile file2(secondFI.absoluteFilePath());

    if (!file1.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка открытия первого файла!";
        return FIRST_FILE_NOT_EXIST;
    }

    if (!file2.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка открытия второго файла!";
        return SECOND_FILE_NOT_EXIST;
    }

    bool fileDiff = false;
    bool breakMain = false;

    QVector<HashGroup> pipe1(_storage.getHash(firstFilePath));
    QVector<HashGroup> pipe2(_storage.getHash(secondFilePath));

    HashGroup tmp;

    while (true && !breakMain)
    {
        if (pipe1.size() == pipe2.size())
        {
            while (!pipe1.empty() && !pipe2.empty())
            {
                if (!(pipe1.takeFirst() == pipe2.takeFirst()))
                {
                    fileDiff = true;
                    breakMain = true;
                    break;
                }
            }
        }
        else if (pipe1.size() > pipe2.size())
        {
            while (pipe1.size() > pipe2.size())
            {
                qint64 offset = pipe2.size() * HASH_BLOCK_SIZE;

                if (file2.size() < offset)
                {
                    fileDiff = true;
                    breakMain = true;
                    break;
                }

                tmp = readHashBlock(file2, offset);

                if (tmp == HashGroup())
                {
                    breakMain = true;
                    break;
                }
                pipe2.push_back(tmp);
                _storage.addHash(secondFilePath, tmp);
            }

            while (!pipe1.empty() && !pipe2.empty())
            {
                if (!(pipe1.takeFirst() == pipe2.takeFirst()))
                {
                    fileDiff = true;
                    breakMain = true;
                    break;
                }
            }
        }
        else
        {
            while (pipe2.size() > pipe1.size())
            {
                qint64 offset = pipe1.size() * HASH_BLOCK_SIZE;

                if (file1.size() < offset)
                {
                    break;
                }

                tmp = readHashBlock(file1, offset);

                if (tmp == HashGroup())
                {
                    breakMain = true;
                    break;
                }
                pipe1.push_back(tmp);
                _storage.addHash(firstFilePath, tmp);
            }

            while (!pipe1.empty() && !pipe2.empty())
            {
                if (!(pipe1.takeFirst() == pipe2.takeFirst()))
                {
                    fileDiff = true;
                    breakMain = true;
                    break;
                }
            }
        }

        qint64 offset1 = _storage.getHashCount(firstFilePath) * HASH_BLOCK_SIZE;
        qint64 offset2 = _storage.getHashCount(secondFilePath) * HASH_BLOCK_SIZE;

        if (file1.size() < offset1)
        {
            break;
        }

        if (file2.size() < offset2)
        {
            break;
        }

        HashGroup tmp1 = readHashBlock(file1, offset1);
        HashGroup tmp2 = readHashBlock(file2, offset2);

        if ((tmp1 == HashGroup()) || (tmp2 == HashGroup()))
            break;

        pipe1.push_back(tmp1);
        _storage.addHash(firstFilePath, pipe1.back());
        pipe2.push_back(tmp2);
        _storage.addHash(secondFilePath, pipe2.back());
    }

    file1.close();
    file2.close();

    if (fileDiff)
    {
        if (firstFI.fileName() != secondFI.fileName())
        {
            return NAMES_DIFF_CONTENT_DIFF;
        }
        else
        {
            return NAMES_SAME_CONTENT_DIFF;
        }
    }
    else
    {
        if (firstFI.fileName() != secondFI.fileName())
        {
            return NAMES_DIFF_CONTENT_SAME;
        }
        else
        {
            return NAMES_SAME_CONTENT_SAME;
        }
    }
}

void LibFCL::reset()
{
    _storage.reset();
}

HashGroup LibFCL::readHashBlock(QFile &file, const qint64 &offset)
{
    QByteArray weakBlock;
    QByteArray strongBlock;
    HashGroup group;

    Q_ASSERT(file.seek(offset));

    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        weakBlock = file.read(BLOCK_SIZE);

        if (weakBlock.size())
        {
            if (weakBlock.size() < BLOCK_SIZE)
            {
                weakBlock = weakBlock.leftJustified(BLOCK_SIZE, '\0', true);
            }
        }
        else
        {
            weakBlock = QByteArray(BLOCK_SIZE, '\0');
        }

        strongBlock.append(weakBlock);
        group.weak[i] = QCryptographicHash::hash(weakBlock, QCryptographicHash::Sha1);
    }
    group.strong = QCryptographicHash::hash(strongBlock, QCryptographicHash::Sha3_256);

    return group;
}

