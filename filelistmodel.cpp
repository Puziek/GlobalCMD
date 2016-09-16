#include "filelistmodel.h"

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&fileWatcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString& path) {
        if (path == QDir::rootPath() &&                               // I hope it's only temporary workaround
            currDirectory.absolutePath() != QDir::rootPath()) return; // to avoid unstoppable root updates
        changeDirectory(path);
    });
}

QString FileListModel::getFileDir(const QModelIndex& index) const
{
    return fileList[index.row()].absoluteFilePath();
}

QString FileListModel::getFileName(const QModelIndex& index) const
{
    return fileList[index.row()].fileName();
}

void FileListModel::copyFiles(const QModelIndexList &selected, QString destDir)
{
    //TODO:: Copy directories
    for (const QModelIndex& index : selected) {
        QString destDir_ = destDir + getFileName(index);
        QString srcDir = getFileDir(index);
        QFile::copy(srcDir, destDir_);
    }
}

void FileListModel::removeFiles(const QModelIndexList &selected)
{
    for (const QModelIndex& index : selected) {
        QString srcDir = getFileDir(index);
        if (!fileList.at(index.row()).isDir()) {
            QFile::remove(srcDir);
        }
        else {
            QDir(srcDir).removeRecursively();
        }
    }
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
                case FileName:
                    if (!file.isFile()) {
                        return QString("[" + file.fileName() + "]");
                    }
                    else {
                        return file.completeBaseName();
                    }

                case Suffix:
                    if (!file.isFile()) {
                        return "<DIR>";
                    }
                    else {
                        return file.suffix();
                    }

                case Size:
                    if (!file.isFile()) {
                        return "\0";
                    }
                    else {
                        return file.size();
                    }

                case Date:
                    return file.created().toString(Qt::LocaleDate);

                case Attributes:
                    return getAttrString(file);
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
                        return iconProvider.icon(file);
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
                return tr("File name");

            case Suffix:
                return tr("Ext");

            case Size:
                return tr("Size");

            case Date:
                return tr("Time");

            case Attributes:
                return tr("Attr");

            default:
                return QVariant();
        }
    }

    return QVariant();
}

void FileListModel::changeDirectory(const QString& newDir)
{
    fileWatcher.removePath(currDirectory.absolutePath());
    fileWatcher.addPath(newDir);
    qDebug() << fileWatcher.directories();

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

QString FileListModel::getAttrString(const QFileInfo& file) const
{
    QString attrString;

    (file.isReadable()) ? attrString += "r" : attrString += "-";
    (file.isWritable()) ? attrString += "w" : attrString += "-";
    (file.isExecutable()) ? attrString += "x" : attrString += "-";
    (file.isHidden()) ? attrString += "h" : attrString += "-";

    return attrString;
}

void FileListModel::changeDirectoryReq(const QModelIndex& index)
{
    changeDirectory(getFileDir(index));
}
