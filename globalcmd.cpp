#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);

    ui->w_rPanel->setBuddyPanel(ui->w_lPanel);
    ui->w_lPanel->setBuddyPanel(ui->w_rPanel);

    connect(ui->w_rPanel, &FileListPanel::directoryChanged, ui->w_lPanel, &FileListPanel::setDirectoryPath);
    connect(ui->w_lPanel, &FileListPanel::directoryChanged, ui->w_rPanel, &FileListPanel::setDirectoryPath);
    connect(ui->w_lPanel, &FileListPanel::focusChanged, this, [this] {
        if (focusedPanel == ui->w_lPanel) return;
        focusedPanel = ui->w_lPanel;
        qDebug() << "[DEBUG] Left panel is active";
    });

    connect(ui->w_rPanel, &FileListPanel::focusChanged, this, [this] {
        if (focusedPanel == ui->w_rPanel) return;
        focusedPanel = ui->w_rPanel;
        qDebug() << "[DEBUG] Right panel is active";
    });

    connect(ui->actionCopy, &QAction::triggered, this, [this] {
        focusedPanel->copyFiles();
    });

    connect(ui->actionRemove, &QAction::triggered, this, [this] {
        focusedPanel->removeFiles();
    });
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}

void GlobalCMD::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
        focusedPanel->goDirUp();
        break;
    case Qt::Key_Delete:
        focusedPanel->removeFiles();
        return;
    case Qt::Key_F5:
        focusedPanel->copyFiles();
        return;
    }

    QWidget::keyPressEvent(event);

}
