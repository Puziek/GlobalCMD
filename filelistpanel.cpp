#include "filelistpanel.h"
#include "ui_filelistpanel.h"
#include <QDebug>

FileListPanel::FileListPanel(QString startingPath, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::FileListPanel)
{
    ui->setupUi(this);
    ui->tv_fileList->setFont(qvariant_cast<QFont> (SettingsManager::getSetting("Fonts", "List font", QFont())));

    if (startingPath == NULL) {
        startingPath = QDir::rootPath();
    }

    fileListModel = new FileListModel();

    ui->tv_fileList->setModel(fileListModel);
    ui->tv_fileList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv_fileList->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tv_fileList->horizontalHeader()->setSectionResizeMode(FileListModel::Columns::FileName, QHeaderView::Stretch);

    ui->l_folderPath->setStyleSheet("QLabel {"
                                    "color: #FFFFFF;"
                                    "background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #d7801a);"
                                    "}");

    connect(ui->tv_fileList, SIGNAL(activated(QModelIndex)), this, SLOT(setDirectory(QModelIndex)));
    connect(ui->tv_fileList, &QTableView::pressed, this, [this] {
        emit focusChanged(this);
    });

    connect(ui->cb_disks, SIGNAL(activated(int)), this, SLOT(changeDriveReq(int)));

    connect(fileListModel, &FileListModel::directoryChanged, this, &FileListPanel::setDirectoryPath);
    connect(fileListModel, &FileListModel::diskSpaceUpdated, this, [this] (const QString& diskSpace) {
       ui->l_diskSpace->setText(diskSpace);
    });

    fileListModel->changeDirectory(startingPath);
    ui->cb_disks->updateDrivesList(QDir(startingPath));
}

FileListPanel::~FileListPanel()
{
    delete ui;
}

void FileListPanel::copyFiles()
{
    QModelIndexList selection = ui->tv_fileList->selectionModel()->selectedRows();
    QString destDir = buddyPanel->fileListModel->getCurrDirectory().absolutePath();
    QFileInfoList filesToCopy;
    bool yesToAll = false;

    if (selection.size() == 0) {
        qDebug() << "[DEBUG copyFiles] selected count = 0";
        return;
    }

    //TODO: Create separate function from this part of code
    for (const QFileInfo& file : fileListModel->getFileInfoFromSelection(selection)) {
        if (QFile(destDir + "/" + file.fileName()).exists() && !yesToAll) {
            int answer = QMessageBox::question(nullptr, tr("Overwrite?"), tr("Do you want to overwrite file: ") + file.fileName(),
                                               QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll);

            if (answer == QMessageBox::NoToAll) break;
            switch(answer) {
            case QMessageBox::Yes:
                filesToCopy.append(file);
                break;

            case QMessageBox::YesToAll:
                yesToAll = true;
                filesToCopy.append(file);
                break;

            case QMessageBox::No:
                break;
            }
        }
        else {
            filesToCopy.append(file);
        }
    }

    FileOperator::startCopyThread(filesToCopy, destDir);
}

void FileListPanel::removeFiles()
{
    QModelIndexList selection = ui->tv_fileList->selectionModel()->selectedRows();

    if (selection.size() == 0) {
        qDebug() << "[DEBUG removeFiles] selected count = 0";
        return;
    }

    FileOperator::startRemoveThread(fileListModel->getFileInfoFromSelection(selection));
}

void FileListPanel::moveFiles()
{
    QModelIndexList selection = ui->tv_fileList->selectionModel()->selectedRows();
    QString destDir = buddyPanel->fileListModel->getCurrDirectory().absolutePath();
    QFileInfoList filesToMove;
    bool yesToAll = false;

    if (selection.size() == 0) {
        qDebug() << "[DEBUG moveFiles] selected count = 0";
        return;
    }

    //TODO: Create separate function from this part of code
    for (const QFileInfo& file : fileListModel->getFileInfoFromSelection(selection)) {
        if (QFile(destDir + "/" + file.fileName()).exists() && !yesToAll) {
            int answer = QMessageBox::question(nullptr, tr("Overwrite?"), tr("Do you want to overwrite file: ") + file.fileName(),
                                               QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll);

            if (answer == QMessageBox::NoToAll) break;
            switch(answer) {
            case QMessageBox::Yes:
                filesToMove.append(file);
                break;

            case QMessageBox::YesToAll:
                yesToAll = true;
                filesToMove.append(file);
                break;

            case QMessageBox::No:
                break;
            }
        }
        else {
            filesToMove.append(file);
        }
    }

    FileOperator::startMoveThread(filesToMove, destDir);
}

void FileListPanel::createDirectory()
{
    QString dirName = QInputDialog::getText(this, tr("Directory name"),
                                            tr("Directory name:"), QLineEdit::Normal,
                                            "New folder");
    FileOperator::createDirectory(fileListModel->getCurrDirectory(), dirName);
}

void FileListPanel::goDirUp()
{
    QDir tempDir = fileListModel->getCurrDirectory();
    tempDir.cdUp();
    fileListModel->changeDirectory(tempDir.absolutePath());
}

void FileListPanel::setBuddyPanel(FileListPanel* buddy)
{
    disconnect(ui->tv_fileList, &GCMDTableView::tabPressed, buddyPanel, nullptr);

    buddyPanel = buddy;

    connect(ui->tv_fileList, &GCMDTableView::tabPressed, buddyPanel, [this] {
        buddyPanel->ui->tv_fileList->setFocus();
        emit buddyPanel->focusChanged(buddyPanel);
    });
}

void FileListPanel::updateFont(const QFont &font)
{
    ui->tv_fileList->setFont(font);
}

void FileListPanel::setHiddenColumns(const QBitArray &hiddenColumns)
{
    for (int column = 0; column < FileListModel::Count; ++column) {
        ui->tv_fileList->setColumnHidden(column, hiddenColumns.at(column));
    }
}

QString FileListPanel::getCurrDirName()
{
    if (fileListModel->getCurrDirectory().isRoot()) {
        return fileListModel->getCurrDirectory().absolutePath();
    }

    return fileListModel->getCurrDirectory().dirName();
}

QString FileListPanel::getCurrDir()
{
    return fileListModel->getCurrDirectory().absolutePath();
}

void FileListPanel::changeDriveReq(int /*index*/)
{
    fileListModel->changeDirectory(ui->cb_disks->currentText());
}

void FileListPanel::setDirectoryPath(const QString &path)
{
    QDir dirPath = QDir(path);
    (dirPath.isRoot()) ? emit dirNameChanged(dirPath.absolutePath()) :
                         emit dirNameChanged(dirPath.dirName());
    ui->l_folderPath->setText(path);
    buddyPath = QDir(path);
}

void FileListPanel::setDirectory(const QModelIndex &index)
{
    fileListModel->changeDirectory(fileListModel->getFileDir(index));
}
