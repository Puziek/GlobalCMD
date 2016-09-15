#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);
    connect(ui->w_rPanel, &FileListPanel::directoryChanged, ui->w_lPanel, &FileListPanel::onDirectoryChanged);
    connect(ui->w_lPanel, &FileListPanel::directoryChanged, ui->w_rPanel, &FileListPanel::onDirectoryChanged);
    connect(ui->actionCopy, &QAction::triggered, this, [this]{
        if (ui->w_lPanel->hasFocus()) {

        } else if (ui->w_rPanel->hasFocus()) {

        } else {
            qDebug() << "Something fucked up";
        }
    });
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}
