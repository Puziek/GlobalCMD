#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);
    fileListModel = new FileListModel();
    ui->tv_lFileList->setModel(fileListModel);
    ui->tv_lFileList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv_lFileList->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tv_lFileList->horizontalHeader()->setSectionResizeMode(FileListModel::Columns::FileName, QHeaderView::Stretch);
    ui->tv_lFileList->horizontalHeader()->setSectionResizeMode(FileListModel::Columns::Suffix, QHeaderView::ResizeToContents);
    ui->tv_lFileList->horizontalHeader()->setSectionResizeMode(FileListModel::Columns::Size, QHeaderView::Stretch);
    ui->cb_lDisks->addItems(fileListModel->getDriversList());

    ui->l_lFolderPath->setStyleSheet("QLabel { background-color : #4374E5; font-size : 20}");

    connect(ui->tv_lFileList, &QTableView::activated, fileListModel, &FileListModel::changeDirectoryReq);
    connect(ui->cb_lDisks, SIGNAL(activated(int)), this, SLOT(changeDriveReq(int)));
    connect(ui->cb_lDisks, &DrivesComboBox::clicked, this, [this] {
        ui->cb_lDisks->clear();
        ui->cb_lDisks->addItems(fileListModel->getDriversList());
    });

    connect(fileListModel, &FileListModel::directoryChanged, this, &GlobalCMD::onDirectoryChanged);

    changeDriveReq();
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}

void GlobalCMD::changeDriveReq(int /*index*/)
{
    QString dir = ui->cb_lDisks->currentText();
    fileListModel->changeDirectory(dir);

    QStorageInfo storage;
    storage.setPath(dir);
    ui->l_lDiskSpace->setText(QString::number(storage.bytesAvailable() / 1000 / 1000) + " MB / "
                              + QString::number(storage.bytesTotal() / 1000 / 1000) + " MB");

}

void GlobalCMD::onDirectoryChanged(const QString& path)
{
    ui->l_lFolderPath->setText(path);
}
