#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QObject>
#include <QFileInfo>
#include <QAbstractListModel>
#include <QDir>
#include <QDebug>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileListModel(QObject *parent = 0);

    enum Columns {
        FileName,
        Suffix,
        Size,
        Count
    };

    QString getFileDir(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void changeDirectory(const QString& newDir);
    QStringList getDriversList();


    QString currDirectory;

signals:

public slots:
    void onChangeDirectoryReq(const QModelIndex& index);

private:
    QFileInfoList fileList;

};

#endif // FILELISTMODEL_H
