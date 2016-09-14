#include "filelistmodel.h"

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent),
      currDirectory("C:/")
{
    QDir directory = QDir(currDirectory);

    fileList = directory.entryInfoList(QDir::AllEntries | QDir::NoDot, QDir::DirsFirst);
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

    if (role == Qt::DisplayRole) {
        const QFileInfo& file = fileList[index.row()];
        switch (index.column()) {
            case FileName:
            {
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
    else if (role == Qt::TextAlignmentRole)
    {
        switch (index.column()) {
            case FileName:
                return Qt::AlignLeft;

            case Suffix:
                return Qt::AlignRight;

            case Size:
                return Qt::AlignRight;
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
    currDirectory = std::move(newDir);
    QDir directory = QDir(currDirectory);
    QFileInfoList tempFileList = directory.entryInfoList(QDir::AllEntries | QDir::NoDot, QDir::DirsFirst);

    beginRemoveRows(QModelIndex(), 0, fileList.size() - 1);
    fileList.clear();
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, tempFileList.size() - 1);
    fileList = std::move(tempFileList);
    endInsertRows();
}

QStringList FileListModel::getDriversList()
{
    QStringList drives;

    for (const QFileInfo& driver : QDir::drives()) {
        drives.append(driver.filePath());
    }

    return drives;
}

void FileListModel::onChangeDirectoryReq(const QModelIndex& index)
{
    changeDirectory(getFileDir(index));
}
