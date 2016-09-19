#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include "filelistmodel.h"
#include <QObject>
#include <QtConcurrent>
#include <QModelIndexList>

class FileOperator : public QObject
{
    Q_OBJECT

public:
    FileOperator();

    static void startCopyThread(const QFileInfoList &filesToRemove, const QString& destDir);
    static void startRemoveThread(const QFileInfoList& filesToRemove);
    static void startMoveThread(const QFileInfoList& filesToCopy, const QString& destDir);
    static void createDirectory(const QDir& directory, const QString& dirName);

private:
};

#endif // FILEOPERATOR_H
