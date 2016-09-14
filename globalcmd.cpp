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
    ui->tv_lFileList->horizontalHeader()->setSectionResizeMode(FileListModel::Columns::Size, QHeaderView::ResizeToContents);
    ui->cb_lDisks->addItems(fileListModel->getDriversList());

    connect(ui->tv_lFileList, &QTableView::activated, fileListModel, &FileListModel::onChangeDirectoryReq);
    connect(ui->cb_lDisks, SIGNAL(activated(int)), this, SLOT(onDiskChange(int)));
    connect(ui->cb_lDisks, &DrivesComboBox::clicked, this, [this] {
        ui->cb_lDisks->clear();
        ui->cb_lDisks->addItems(fileListModel->getDriversList());
    });
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}

void GlobalCMD::onDiskChange(int index)
{
    QString dir = ui->cb_lDisks->currentText();
    fileListModel->changeDirectory(dir);
}
