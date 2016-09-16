#include "filelistpanel.h"
#include "ui_filelistpanel.h"

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

    if (selection.size() == 0) {
        qDebug() << "[DEBUG copyFiles] selected count = 0";
        return;
    }

    //TODO:: Copy directories
    for (const QModelIndex& index : selection) {
        QString destDir = buddyPanel->fileListModel->getCurrDirectory().absolutePath();
        QString srcDir = fileListModel->getFileDir(index);
        //TODO: Check if there is any way to make it more unified (/ for unix, \ for windows
        (destDir.endsWith("/")) ? destDir += fileListModel->getFileName(index) : destDir = destDir + "/" + fileListModel->getFileName(index);
        qDebug() << "SRC: " << srcDir;
        qDebug() << "DST: " << destDir;
        QFile::copy(srcDir, destDir);
    }
}

void FileListPanel::removeFiles()
{
    QModelIndexList selection = ui->tv_fileList->selectionModel()->selectedRows();

    if (selection.size() == 0) {
        qDebug() << "[DEBUG removeFiles] selected count = 0";
        return;
    }

    for (const QModelIndex& index : selection) {
        QString srcDir = fileListModel->getFileDir(index);
        if (!fileListModel->isDir(index)) {
            QFile::remove(srcDir);
        }
        else {
            QDir(srcDir).removeRecursively();
        }
    }
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
