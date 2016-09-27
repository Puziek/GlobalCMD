#include "fileoperator.h"

FileOperator::FileOperator()
{

}

void FileOperator::startCopyThread(const QFileInfoList& filesToCopy, const QString& destDir)
{ 
    //TODO:: Copy directories
    QtConcurrent::run([](const QFileInfoList& filesToCopy, const QString& destDir){
        for (const QFileInfo& file : filesToCopy) {
            QString dest = destDir;
            (dest.endsWith(QDir::separator())) ? dest += file.fileName() :
                                   dest = dest + QDir::separator() + file.fileName();
            if (QFile(dest).exists()) {
                QFile::remove(dest);
            }
            QFile::copy(file.absoluteFilePath(), dest);
        }
    }, filesToCopy, destDir);
}

void FileOperator::startRemoveThread(const QFileInfoList& filesToRemove)
{
    QtConcurrent::run([](const QFileInfoList& filesToRemove) {
    for (const QFileInfo& file : filesToRemove) {
            if (!file.isDir()) {
                QFile::remove(file.absoluteFilePath());
            }
            else {
                QDir(file.absoluteFilePath()).removeRecursively();
            }
        }
    }, filesToRemove);
}

void FileOperator::startMoveThread(const QFileInfoList& filesToMove, const QString& destDir)
{
    QtConcurrent::run([](const QFileInfoList& filesToMove, const QString& destDir){
        for (const QFileInfo& file : filesToMove) {
            QString dest = destDir;
            (dest.endsWith(QDir::separator())) ? dest += file.fileName() :
                                   dest = dest + QDir::separator() + file.fileName();
            QFile::rename(file.absoluteFilePath(), dest);
        }
    }, filesToMove, destDir);
}

void FileOperator::createDirectory(const QDir& directory, const QString& dirName)
{
    directory.mkdir(dirName);
}

