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

    connect(ui->tv_fileList, &QTableView::activated, fileListModel, &FileListModel::changeDirectoryReq);
    connect(ui->cb_disks, SIGNAL(activated(int)), this, SLOT(changeDriveReq(int)));
    connect(ui->cb_disks, &DrivesComboBox::clicked, this, [this] {
        ui->cb_disks->clear();
        ui->cb_disks->addItems(fileListModel->getDriversList());
    });

    connect(fileListModel, &FileListModel::directoryChanged, this, &FileListPanel::onDirectoryChanged);

    changeDriveReq();
}

FileListPanel::~FileListPanel()
{
    delete ui;
}

void FileListPanel::changeDriveReq(int index)
{
    QString dir = ui->cb_disks->currentText();
    fileListModel->changeDirectory(dir);

    QStorageInfo storage;
    storage.setPath(dir);
    ui->l_diskSpace->setText(QString::number(storage.bytesAvailable() / 1000 / 1000) + " MB / "
                              + QString::number(storage.bytesTotal() / 1000 / 1000) + " MB");
}

void FileListPanel::onDirectoryChanged(const QString &path)
{
    ui->l_folderPath->setText(path);
    buddyPath = QDir(path);
    qDebug() << buddyPath;
}
