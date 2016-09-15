#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);

    ui->w_rPanel->setBuddyPanel(ui->w_lPanel);
    ui->w_lPanel->setBuddyPanel(ui->w_rPanel);

    connect(ui->w_rPanel, &FileListPanel::directoryChanged, ui->w_lPanel, &FileListPanel::onDirectoryChanged);
    connect(ui->w_lPanel, &FileListPanel::directoryChanged, ui->w_rPanel, &FileListPanel::onDirectoryChanged);
    connect(ui->w_lPanel, &FileListPanel::focusChanged, this, [this] {
        if (!isLeftPanelFocused) {
            isLeftPanelFocused = true;
            qDebug() << "Left panel is active";
        }
    });
    connect(ui->w_rPanel, &FileListPanel::focusChanged, this, [this] {
        if (isLeftPanelFocused) {
            isLeftPanelFocused = false;
            qDebug() << "Right panel is active";
        }
    });
    connect(ui->actionCopy, &QAction::triggered, this, [this] {
        if (isLeftPanelFocused) {
            ui->w_lPanel->copyFile();
        }
    });
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}
