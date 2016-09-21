#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include "fileoperator.h"
#include <QObject>
#include <QFileInfo>
#include <QAbstractListModel>
#include <QDir>
#include <QDebug>
#include <QFileIconProvider>
#include <QDateTime>
#include <QFileSystemWatcher>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileListModel(QObject *parent = 0);

    enum Columns {
        FileName,
        Suffix,
        Size,
        Date,
        Attributes,
        Count
    };


    bool isDir(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void changeDirectory(const QString& newDir);
    QString getAttrString(const QFileInfo &file) const;
    QString getFileDir(const QModelIndex& index) const;
    QString getFileName(const QModelIndex& index) const;
    QDir getCurrDirectory();
    QFileInfoList getFileInfoFromSelection(const QModelIndexList& selection);
    void setCurrDirectory(const QDir& path);

signals:
    void directoryChanged(const QString& path);
    void diskSpaceUpdated(const QString& string);

public slots:

private:
    QDir currDirectory;
    QFileInfoList fileList;
    QFileSystemWatcher fileWatcher;

};

#endif // FILELISTMODEL_H
