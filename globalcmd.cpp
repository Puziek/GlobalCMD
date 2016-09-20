#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);

    QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+T")), this);

    // workaround

    ui->tw_leftPanel->removeTab(1);
    ui->tw_rightPanel->removeTab(1);

    updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                      ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                      true);
    //

    connect(dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex())),
            &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
        focusedPanel = focused;
    });

    connect(dynamic_cast<FileListPanel*> (ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex())),
            &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
        focusedPanel = focused;
    });

    connect(ui->tw_leftPanel, &QTabWidget::currentChanged, this, [this] {
        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          true);
    });

    connect(ui->tw_rightPanel, &QTabWidget::currentChanged, this, [this] {
        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          false);
    });

    connect(ui->actionCopy, &QAction::triggered, this, [this] {
        focusedPanel->copyFiles();
    });

    connect(ui->actionRemove, &QAction::triggered, this, [this] {
        focusedPanel->removeFiles();
    });

    connect(shortcut, QShortcut::activated, this, GlobalCMD::createNewTab);
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}

void GlobalCMD::createNewTab()
{
    if (focusedPanel == dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()))) {
        ui->tw_leftPanel->addTab(new FileListPanel(), "TEST");
        ui->tw_leftPanel->setCurrentIndex(ui->tw_leftPanel->count() - 1);

        disconnect(dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex())),
                &FileListPanel::focusChanged, this, nullptr);

        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          true);

        connect(dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex())),
                &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
            focusedPanel = focused;
        });
    }
    else {
        ui->tw_rightPanel->addTab(new FileListPanel(), "TEST");
        ui->tw_rightPanel->setCurrentIndex(ui->tw_rightPanel->count() - 1);

        disconnect(dynamic_cast<FileListPanel*> (ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex())),
                   &FileListPanel::focusChanged, this, nullptr);

        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          false);

        connect(dynamic_cast<FileListPanel*> (ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex())),
                &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
            focusedPanel = focused;
        });
    }
}

void GlobalCMD::updateCurrentTabs(QWidget* firstPanel, QWidget* secondPanel, bool isFirstFocused)
{
    FileListPanel* firstListPanel = dynamic_cast<FileListPanel*> (firstPanel);
    FileListPanel* secondListPanel = dynamic_cast<FileListPanel*> (secondPanel);

    firstListPanel->setBuddyPanel(secondListPanel);
    secondListPanel->setBuddyPanel(firstListPanel);

    (isFirstFocused) ? focusedPanel = firstListPanel : focusedPanel = secondListPanel;
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

    case Qt::Key_F6:
        focusedPanel->moveFiles();
        return;

    case Qt::Key_F7:
        focusedPanel->createDirectory();
        return;
    }

    QWidget::keyPressEvent(event);
}
