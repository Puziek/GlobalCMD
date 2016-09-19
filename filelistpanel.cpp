#include "filelistpanel.h"
#include "ui_filelistpanel.h"
#include <QDebug>

FileListPanel::FileListPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileListPanel)
{
    ui->setupUi(this);

    fileListModel = new FileListModel();
    ui->tv_fileList->setModel(fileListModel);
    ui->tv_fileList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv_fileList->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tv_fileList->horizontalHeader()->setSectionResizeMode(FileListModel::Columns::FileName, QHeaderView::Stretch);

    ui->cb_disks->addItems(fileListModel->getDriversList());
    ui->cb_disks->showPopup();

    ui->l_folderPath->setStyleSheet("QLabel { background-color : #4374E5; font-size : 20}");

    connect(ui->tv_fileList, SIGNAL(activated(QModelIndex)), this, SLOT(setDirectory(QModelIndex)));
    connect(ui->tv_fileList, &QTableView::pressed, this, [this] {
        emit focusChanged();
    });

    connect(ui->cb_disks, SIGNAL(activated(int)), this, SLOT(changeDriveReq(int)));
    connect(ui->cb_disks, &DrivesComboBox::clicked, this, [this] {
        ui->cb_disks->clear();
        ui->cb_disks->addItems(fileListModel->getDriversList());
    });

    connect(fileListModel, &FileListModel::directoryChanged, this, &FileListPanel::setDirectoryPath);

    changeDriveReq();
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
    qDebug() << tempDir.absolutePath();
    fileListModel->changeDirectory(tempDir.absolutePath());
}

void FileListPanel::setBuddyPanel(FileListPanel* buddy)
{
    buddyPanel = buddy;
    connect(ui->tv_fileList, &GCMDTableView::tabPressed, buddyPanel, [this] {
        buddyPanel->ui->tv_fileList->setFocus();
        emit buddyPanel->focusChanged();
    });
}

void FileListPanel::changeDriveReq(int /*index*/)
{
    fileListModel->changeDirectory(ui->cb_disks->currentText());

    QStorageInfo storage(ui->cb_disks->currentText());
    ui->l_diskSpace->setText(QString::number(storage.bytesAvailable() / 1000 / 1000) + " MB / "
                             + QString::number(storage.bytesTotal() / 1000 / 1000) + " MB");
}

void FileListPanel::setDirectoryPath(const QString &path)
{
    ui->l_folderPath->setText(path);
    buddyPath = QDir(path);
}

void FileListPanel::setDirectory(const QModelIndex &index)
{
    fileListModel->changeDirectory(fileListModel->getFileDir(index));
}
