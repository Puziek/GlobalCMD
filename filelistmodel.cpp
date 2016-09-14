#include "filelistmodel.h"

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QString FileListModel::getFileDir(const QModelIndex &index) const
{
    return fileList[index.row()].absoluteFilePath();
}

int FileListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return fileList.size();
}

int FileListModel::columnCount(const QModelIndex& /*parent*/) const
{
    return Columns::Count;
}

QVariant FileListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= fileList.size()) {
        return QVariant();
    }

    QFileIconProvider iconProvider;
    const QFileInfo& file = fileList[index.row()];

    switch (role) {
        case Qt::DisplayRole:
        {
            switch (index.column()) {
                case FileName: {
                    if (!file.isFile()) {
                        return QString("[" + file.fileName() + "]");
                    }
                    else {
                        return file.completeBaseName();
                    }
                }
                case Suffix:
                {
                    if (!file.isFile()) {
                        return "<DIR>";
                    }
                    else {
                        return file.suffix();
                    }
                }
                case Size:
                {
                    if (!file.isFile()) {
                        return "\0";
                    }
                    else {
                        return file.size();
                    }
                }
            }
        }
        case Qt::TextAlignmentRole: {
            switch (index.column()) {
                case FileName:
                    return Qt::AlignLeft;

                case Suffix:
                    return Qt::AlignRight;

                case Size:
                    return Qt::AlignRight;
            }
        }
        case Qt::DecorationRole: {
            switch (index.column()) {
                case FileName:
                {
                    if (!file.isFile()) {
                        return iconProvider.icon(QFileIconProvider::Folder);
                    }
                    else {
                        return iconProvider.icon(QFileIconProvider::File);
                    }
                }
            }
        }
    }

    return QVariant();
}

QVariant FileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case FileName:
            return "File name";

        case Suffix:
            return "Ext";

        case Size:
            return "Size";

        default:
            return QVariant();
        }
    }

    return QVariant();
}

void FileListModel::changeDirectory(const QString& newDir)
{
    currDirectory = QDir(newDir);

    beginResetModel();
    fileList = currDirectory.entryInfoList(QDir::AllEntries | QDir::NoDot, QDir::DirsFirst);
    endResetModel();

    emit directoryChanged(currDirectory.absolutePath());
}

QStringList FileListModel::getDriversList()
{
    QStringList drives;

    for (const QFileInfo& driver : QDir::drives()) {
        drives.append(driver.filePath());
    }

    return drives;
}

void FileListModel::changeDirectoryReq(const QModelIndex& index)
{
    changeDirectory(getFileDir(index));
}
