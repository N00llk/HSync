#include "DIRSync.h"

DIRSync::DIRSync()
{

}

void DIRSync::sync(QString source, QString dest)
{
    QDirIterator sourceDir(source, QDirIterator::Subdirectories);
    QDirIterator destDir(dest, QDirIterator::Subdirectories);

    QVector<QString> files1;
    QVector<QString> files2;

    QFileInfo info;
    while (sourceDir.hasNext())
    {
        info.setFile(sourceDir.next());
        if (info.isFile())
        {
            files1.push_back(info.absoluteFilePath());
        }
    }

    while (destDir.hasNext())
    {
        info.setFile(destDir.next());
        if (info.isFile())
        {
            files2.push_back(info.absoluteFilePath());
        }
    }

    LibFCL fc;
    double progress = files1.size() * files2.size();
    QVector<QVector<int>> result;

    result.clear();
    result.resize(files1.size());

    for (int i = 0; i < files1.size(); i++)
    {
        QString path1 = files1[i];
        result[i].resize(files2.size());
        for (int j = 0; j < files2.size(); j++)
        {
            int progressValue = (i * files2.size() + j) / progress * 100;
            emit updateProgressBar(progressValue);
            QString path2 = files2[j];
            result[i][j] = fc.compare(path1, path2);
        }
    }

    QDir sDir(source);
    QDir dDir(dest);

    for (int i = 0; i < result.size(); i++)
    {
        QString path1 = files1[i];

        if ((result[i].indexOf(NAMES_SAME_CONTENT_SAME) == -1)
           && (result[i].indexOf(NAMES_DIFF_CONTENT_SAME) == -1)
           && (result[i].indexOf(NAMES_SAME_CONTENT_DIFF) == -1))
        {
            copyFile(source, dest, sDir.relativeFilePath(path1));
            emit addNewFilePair(path1, dest + "/" + sDir.relativeFilePath(path1), FILE_HAS_BEEN_COPIED);
            continue;
        }

        int found = result[i].indexOf(NAMES_SAME_CONTENT_SAME);
        while (found != -1)
        {
            if (sDir.relativeFilePath(path1) == dDir.relativeFilePath(files2[found]))
            {
                emit addNewFilePair(path1, files2[found], FILE_EXIST);
                break;
            }
            else
            {
                found = result[i].indexOf(NAMES_SAME_CONTENT_SAME, found);
            }
        }

        if ((result[i].indexOf(NAMES_DIFF_CONTENT_DIFF) == -1)
           && (result[i].indexOf(NAMES_SAME_CONTENT_SAME) == -1)
           && (result[i].indexOf(NAMES_DIFF_CONTENT_SAME) == -1))
        {
            QDir tmp = dDir;
            tmp.cdUp();
            bool res = tmp.mkpath(dDir.dirName() + "_NAMES_SAME_CONTENT_DIFF/");
            Q_ASSERT(res);
            moveFile(dDir.path(), tmp.path() + "/" + dDir.dirName() + "_NAMES_SAME_CONTENT_DIFF/", dDir.relativeFilePath(files2[i]));

            moveFile(source, dest, sDir.relativeFilePath(path1));
            emit addNewFilePair(path1, dest + "/" + sDir.relativeFilePath(path1), FILE_HAS_BEEN_MOVED);
            continue;
        }
    }

    for (int j = 0; j < files2.size(); j++)
    {
        bool flag = true;
        for (int i = 0; i < files1.size(); i++)
        {
            if (result[i][j] != NAMES_DIFF_CONTENT_DIFF)
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            QDir tmp = dDir;
            tmp.cdUp();
            bool res = tmp.mkpath(dDir.dirName() + "_NAMES_DIFF_CONTENT_DIFF/");
            Q_ASSERT(res);
            moveFile(dDir.path(), tmp.path() + "/" + dDir.dirName() + "_NAMES_DIFF_CONTENT_DIFF/", dDir.relativeFilePath(files2[j]));
        }
    }

    for (int j = 0; j < files2.size(); j++)
    {
        bool flag = true;
        for (int i = 0; i < files1.size(); i++)
        {
            if ((result[i][j] == NAMES_DIFF_CONTENT_DIFF)
               || (result[i][j] == NAMES_SAME_CONTENT_SAME))
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            for (int i = 0; i < files1.size(); i++)
            {
                if ((result[i][j] == NAMES_DIFF_CONTENT_DIFF)
                   || (result[i][j] == NAMES_SAME_CONTENT_SAME))
                {
                    flag = false;
                    break;
                }
            }

            QDir tmp = dDir;
            tmp.cdUp();
            bool res = tmp.mkpath(dDir.dirName() + "_NAMES_DIFF_CONTENT_DIFF/");
            Q_ASSERT(res);
            moveFile(dDir.path(), tmp.path() + "/" + dDir.dirName() + "_NAMES_DIFF_CONTENT_DIFF/", dDir.relativeFilePath(files2[j]));
        }
    }

//    QVector<int> duplicates;

//    qDebug() << QDir(sourceDir.path()).relativeFilePath(path1);

//    if (std::find_if(result.begin(), result.end(), [](const int& s){ return (s == NAMES_SAME_CONTENT_SAME); }) != result.end())
//    {

//        result.indexOf()
//    }
//    else if (std::find_if(result.begin(), result.end(), [](const int& s){ return (s == NAMES_SAME_CONTENT_DIFF); }) != result.end())
//    {

//    }

//    for (int k = 0; k < result.size(); k++)
//    {
//        if (result[k] == NAMES_DIFF_CONTENT_SAME || result[k] == NAMES_SAME_CONTENT_SAME)
//        {
//            duplicates.push_back(k);

//        }
//    }
    emit updateProgressBar(100);
}

void DIRSync::sync2(QString baseSource, QString baseDest, QString curSource, QString curDest, int level)
{
    QDirIterator sourceDir(curSource);
    QDirIterator destDir(curDest);

    QVector<QString> files1;
    QVector<QString> files2;
    QVector<QString> dirs1;
    QVector<QString> dirs2;
    QVector<QVector<int>> result;

    QFileInfo info;

    if (level == 0)
    {
        emit updateProgressBar(0);
    }

    while (sourceDir.hasNext())
    {
        info.setFile(sourceDir.next());
        if (info.isFile())
        {
            files1.push_back(info.absoluteFilePath());
        }
        else
        {
            dirs1.push_back(QDir(info.absoluteFilePath()).dirName());
        }
    }

    while (destDir.hasNext())
    {
        info.setFile(destDir.next());
        if (info.isFile())
        {
            files2.push_back(info.absoluteFilePath());
        }
        else
        {
            dirs2.push_back(QDir(info.absoluteFilePath()).dirName());
        }
    }

    dirs1.removeFirst();
    dirs1.removeFirst();
    dirs2.removeFirst();
    dirs2.removeFirst();

    Q_ASSERT(QDir().exists(curSource));
    Q_ASSERT(QDir().exists(curDest));
    QDir sDir = QDir(curSource);
    QDir dDir = QDir(curDest);

    LibFCL fc;
    result.resize(files1.size());
    for (int i = 0; i < files1.size(); i++)
    {
        result[i].resize(files2.size());
        for (int j = 0; j < files2.size(); j++)
        {
            result[i][j] = fc.compare(files1[i], files2[j]);
        }
    }

    qDebug() << result;

    if (files2.size() == 0 && files1.size() != 0)
    {
        for (auto &file: files1)
        {
            copyFile(sDir.path(), dDir.path(), sDir.relativeFilePath(file));
        }
    }

    if (level == 0)
    {
        emit updateProgressBar(10);
    }

    int i = 0;
    int timeoutChanges = 0;
    while (!result.empty() && (timeoutChanges <= (result.size() * 2)))
    {
        i %= result.size();
        if (result[i].indexOf(NAMES_SAME_CONTENT_SAME) != -1)
        {
            QString path1 = files1[i];
            removeFilePair(files1, files2, result, i, result[i].indexOf(NAMES_SAME_CONTENT_SAME));

            emit addNewFilePair(path1, dDir.path() + QDir::separator() + sDir.relativeFilePath(path1), FILE_EXIST);

            timeoutChanges = 0;
            continue;
        }

        if ((result[i].indexOf(NAMES_SAME_CONTENT_DIFF) != -1)
                && (result[i].indexOf(NAMES_DIFF_CONTENT_SAME) == -1))
        {
            QString path1 = files1[i];
            QString path2 = files2[result[i].indexOf(NAMES_SAME_CONTENT_DIFF)];

            moveFile(dDir.path(), baseDest + "_NAMES_SAME_CONTENT_DIFF", dDir.relativeFilePath(path2));
            copyFile(sDir.path(), dDir.path(), sDir.relativeFilePath(path1));
            emit addNewFilePair(path1, dDir.path() + QDir::separator() + sDir.relativeFilePath(path1), FILE_HAS_BEEN_COPIED);

            removeFilePair(files1, files2, result, i, result[i].indexOf(NAMES_SAME_CONTENT_DIFF));
            timeoutChanges = 0;
            continue;
        }

//        if ((result[i].indexOf(NAMES_SAME_CONTENT_DIFF) == -1)
//                && (result[i].indexOf(NAMES_DIFF_CONTENT_SAME) != -1))
//        {
//            QString path1 = files1[i];

//            QFile::rename(path1, dDir.path() + QDir::separator() + sDir.relativeFilePath(path1));

//            timeoutChanges = 0;
//            continue;
//        }

        if ((result[i].indexOf(NAMES_SAME_CONTENT_SAME) == -1)
                && (result[i].indexOf(NAMES_DIFF_CONTENT_SAME) == -1)
                && (result[i].indexOf(NAMES_SAME_CONTENT_DIFF) == -1))
        {
            QString path1 = files1[i];
            copyFile(sDir.path(), dDir.path(), sDir.relativeFilePath(path1));
            emit addNewFilePair(path1, dDir.path() + QDir::separator() + sDir.relativeFilePath(path1), FILE_HAS_BEEN_COPIED);
            result.remove(i);
            files1.remove(i);
            i++;
            timeoutChanges = 0;
            continue;
        }

        i++;
        timeoutChanges++;
    }

    while (!files2.empty())
    {
        QString path2 = files2.takeLast();
        moveFile(dDir.path(), baseDest + "_NAMES_DIFF_CONTENT_DIFF", dDir.relativeFilePath(path2));
        emit addNewFilePair(dDir.path() + QDir::separator() + dDir.relativeFilePath(path2), baseDest + "_NAMES_DIFF_CONTENT_DIFF" + QDir::separator() + dDir.relativeFilePath(path2), FILE_HAS_BEEN_MOVED);
    }

//    while (result.size() != 0)
//    {
//        QString path2 = files2.at(i);
//        if ((result[i].indexOf(NAMES_SAME_CONTENT_SAME) == -1) && (result[i].indexOf(NAMES_DIFF_CONTENT_SAME) == -1) && (result[i].indexOf(NAMES_SAME_CONTENT_DIFF) == -1))
//        {
//            moveFile(sDir.path(), dDir.path(), sDir.relativeFilePath(path2));
//            //emit addNewFilePair(path1, dDir.path() + QDir::separator() + sDir.relativeFilePath(path1), FILE_HAS_BEEN_COPIED);
//            continue;
//    }


//        i++;
//        i %= result.size();
//    }



//        int found = result[i].indexOf(NAMES_SAME_CONTENT_SAME);
//        while (found != -1)
//        {
//            if (sDir.relativeFilePath(path1) == dDir.relativeFilePath(files2[found]))
//            {
//                emit addNewFilePair(path1, files2[found], FILE_EXIST);
//                break;
//            }
//            else
//            {
//                found = result[i].indexOf(NAMES_SAME_CONTENT_SAME, found);
//            }
//        }
//    }

    if (level == 0)
    {
        emit updateProgressBar(60);
    }

    for (int i = 0; i < dirs1.size(); i++)
    {
        bool dir_found = false;
        for (int j = 0; j < dirs2.size(); j++)
        {
            if (dirs1[i] == dirs2[j])
            {
                dir_found = true;
                sync2(baseSource, baseDest, curSource + QDir::separator() + dirs1[i], curDest + QDir::separator() + dirs2[j], level + 1);
                break;
            }
        }

        if (!dir_found)
        {
            bool result = dDir.mkdir(dirs1[i]);
            Q_ASSERT(result);
            sync2(baseSource, baseDest, curSource + QDir::separator() + dirs1[i], curDest + QDir::separator() + dirs1[i], level + 1);
        }
    }

    if (level == 0)
    {
        emit updateProgressBar(80);
    }

    for (int i = 0; i < dirs2.size(); i++)
    {
        if (dirs1.indexOf(dirs2[i]) == -1)
        {
            QDir dir;
            QDir base(baseDest);
            base.cdUp();
            base.mkpath(baseDest + "_DIFF_NAME_DIFF_CONTENT" + QDir::separator() + dirs2[i]);
            QString src = curDest + QDir::separator() + dirs2[i];
            QString dst = baseDest + "_DIFF_NAME_DIFF_CONTENT" + QDir::separator() + dirs2[i];
            bool res = dir.rename(src, dst);
            Q_ASSERT(res);
        }
    }

    if (level == 0)
    {
        emit updateProgressBar(100);
    }
}

void DIRSync::reset()
{
    //fc.reset();
}

void DIRSync::moveFile(QString baseFrom, QString baseTo, QString relFile)
{
    QDir from(baseFrom);
    QDir to(baseTo);
    to.cdUp();
    to.mkdir(baseTo);
    to.cd(baseTo);
    QDir tmpDir(to.path());

    int nat = relFile.lastIndexOf(QDir::separator());
    if (nat != -1)
    {
        tmpDir.mkpath(relFile.mid(0, nat));
    }

    QFile::rename(baseFrom + "/" + relFile, baseTo + "/" + relFile);
}

void DIRSync::copyFile(QString baseFrom, QString baseTo, QString relFile)
{
    QDir from(baseFrom);
    QDir to(baseTo);

    QStringList pathList = relFile.split("/");
    pathList.removeLast();

    QDir tmpDir(to.path());
    for (int i = 0; i < pathList.size(); i++)
    {
        QDir tmp = QDir(tmpDir.path() + "/" + pathList[i]);
        if (!tmp.exists())
        {
            tmpDir.mkdir(pathList[i]);
        }
        tmpDir.setPath(tmpDir.path() + "/" + pathList[i]);
    }

    QFile::copy(baseFrom + "/" + relFile, baseTo + "/" + relFile);
}

void DIRSync::removeFilePair(QVector<QString> &files1, QVector<QString> &files2, QVector<QVector<int> > &result, int first, int second)
{
    files1.remove(first);
    files2.remove(second);

    removeColumn(result, second);
    result.remove(first);
    cleanUp(result);
}

void DIRSync::removeColumn(QVector<QVector<int> > &data, qint64 column)
{
    for (auto &x: data)
    {
        x.remove(column);
    }
}

void DIRSync::cleanUp(QVector<QVector<int> > &data)
{
    int i = 0;
    while (i < data.size())
    {
        if (data[i].empty())
        {
            data.remove(i);
        }
        else
        {
            i++;
        }
    }
}
