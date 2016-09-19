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

bool FileListModel::isDir(const QModelIndex &index) const
{
    if (index.row() > fileList.size()) {
        qDebug() << "[DEBUG isDir] index.row() is overflowing fileList";
        return NULL;
    }
    return fileList.at(index.row()).isDir();
}

QString FileListModel::getFileDir(const QModelIndex& index) const
{
    if (index.row() > fileList.size()) {
        qDebug() << "[DEBUG getFileDir] index.row() is overflowing fileList";
        return NULL;
    }
    return fileList.at(index.row()).absoluteFilePath();
}

QString FileListModel::getFileName(const QModelIndex& index) const
{
    if (index.row() > fileList.size()) {
        qDebug() << "[DEBUG getFileName] index.row() is overflowing fileList";
        return NULL;
    }
    return fileList.at(index.row()).fileName();
}

QDir FileListModel::getCurrDirectory()
{
    return currDirectory;
}

QFileInfoList FileListModel::getFileInfoFromSelection(const QModelIndexList &selection)
{
    QFileInfoList filesFromSelection;

    for (const QModelIndex& index : selection) {
        filesFromSelection.append(fileList.at(index.row()));
    }

    return filesFromSelection;
}

void FileListModel::setCurrDirectory(const QDir &path)
{
    currDirectory = path;
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
    const QFileInfo& file = fileList.at(index.row());

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
                    return file.isFile() ? file.suffix() : "<DIR>";

                case Size:
                    return file.isFile() ? QString::number(file.size()) : "\0";

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

