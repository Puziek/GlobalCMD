#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);

    QShortcut* addTabShortcut = new QShortcut(QKeySequence(tr("Ctrl+T")), this);
    QShortcut* removeTabShortcut = new QShortcut(QKeySequence(tr("Ctrl+W")), this);

    ui->tw_leftPanel->removeTab(1);
    ui->tw_rightPanel->removeTab(1);
    ui->tw_leftPanel->setTabText(0, dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(0))->getCurrDirName());
    ui->tw_rightPanel->setTabText(0, dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(0))->getCurrDirName());

    updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                      ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                      true);

    connect(dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex())),
            &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
        focusedPanel = focused;
        qDebug() << "Focus on: " << focusedPanel->getCurrDirName();
    });

    connect(dynamic_cast<FileListPanel*> (ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex())),
            &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
        focusedPanel = focused;
        qDebug() << "Focus on: " << focusedPanel->getCurrDirName();
    });

    connect(dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex())),
            &FileListPanel::dirNameChanged, this, [this] (const QString& dirName) {
        ui->tw_leftPanel->setTabText(ui->tw_leftPanel->currentIndex(), dirName);
    });

    connect(dynamic_cast<FileListPanel*> (ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex())),
            &FileListPanel::dirNameChanged, this, [this] (const QString& dirName) {
        ui->tw_rightPanel->setTabText(ui->tw_rightPanel->currentIndex(), dirName);
    });

    connect(ui->tw_leftPanel, &QTabWidget::currentChanged, this, [this] {
        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          true);
        qDebug() << "Focus on: " << focusedPanel->getCurrDirName();
    });

    connect(ui->tw_rightPanel, &QTabWidget::currentChanged, this, [this] {
        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          false);
        qDebug() << "Focus on: " << focusedPanel->getCurrDirName();
    });

    connect(ui->actionCopy, &QAction::triggered, this, [this] {
        focusedPanel->copyFiles();
    });

    connect(ui->actionRemove, &QAction::triggered, this, [this] {
        focusedPanel->removeFiles();
    });

    connect(addTabShortcut, QShortcut::activated, this, GlobalCMD::createNewTab);
    connect(removeTabShortcut, QShortcut::activated, this, GlobalCMD::removeCurrentTab);
}

GlobalCMD::~GlobalCMD()
{
    delete ui;
}

void GlobalCMD::createNewTab()
{
    FileListPanel* panel = dynamic_cast<FileListPanel*> (ui->tw_leftPanel->
                                                         widget(ui->tw_leftPanel->currentIndex()));

    if (focusedPanel == panel) {
        ui->tw_leftPanel->addTab(new FileListPanel(panel->getCurrDir()), panel->getCurrDirName());
        ui->tw_leftPanel->setCurrentIndex(ui->tw_leftPanel->count() - 1);

        panel = dynamic_cast<FileListPanel*> (ui->tw_leftPanel->
                                              widget(ui->tw_leftPanel->currentIndex()));

        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          true);
        connectPanelSignals(panel, true);
    }
    else {
        panel = dynamic_cast<FileListPanel*> (ui->tw_rightPanel->
                                              widget(ui->tw_rightPanel->currentIndex()));
        ui->tw_rightPanel->addTab(new FileListPanel(panel->getCurrDir()), panel->getCurrDirName());
        ui->tw_rightPanel->setCurrentIndex(ui->tw_rightPanel->count() - 1);

        panel = dynamic_cast<FileListPanel*> (ui->tw_rightPanel->
                                              widget(ui->tw_rightPanel->currentIndex()));

        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          false);
        connectPanelSignals(panel, false);
    }
}

void GlobalCMD::removeCurrentTab()
{
    FileListPanel* panel = dynamic_cast<FileListPanel*> (ui->tw_leftPanel->
                                                         widget(ui->tw_leftPanel->currentIndex()));
    if (focusedPanel == panel) {
        if (ui->tw_leftPanel->count() == 1) return;
        ui->tw_leftPanel->removeTab(ui->tw_leftPanel->currentIndex());
        delete panel;

        panel = dynamic_cast<FileListPanel*> (ui->tw_leftPanel->
                                              widget(ui->tw_leftPanel->currentIndex()));

        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          true);

        connectPanelSignals(panel, true);
    }
    else {
        if (ui->tw_rightPanel->count() == 1) return;
        panel = dynamic_cast<FileListPanel*> (ui->tw_rightPanel->
                                              widget(ui->tw_rightPanel->currentIndex()));
        ui->tw_rightPanel->removeTab(ui->tw_rightPanel->currentIndex());
        delete panel;

        panel = dynamic_cast<FileListPanel*> (ui->tw_rightPanel->
                                              widget(ui->tw_rightPanel->currentIndex()));

        updateCurrentTabs(ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex()),
                          ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex()),
                          false);

        connectPanelSignals(panel, false);
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

void GlobalCMD::connectPanelSignals(FileListPanel* panel, bool isFirstFocused)
{
    disconnect(panel, &FileListPanel::focusChanged, this, nullptr);   // Is there any way to check
    disconnect(panel, &FileListPanel::dirNameChanged, this, nullptr); // if connection exists?

    connect(panel, &FileListPanel::focusChanged, this, [this] (FileListPanel* focused) {
        focusedPanel = focused;
        qDebug() << "Focus on: " << focusedPanel->getCurrDirName();
    });

    if (isFirstFocused) {
        connect(panel, &FileListPanel::dirNameChanged, this, [this] (const QString& dirName) {
            ui->tw_leftPanel->setTabText(ui->tw_leftPanel->currentIndex(), dirName);
        });
    }
    else {
        connect(panel, &FileListPanel::dirNameChanged, this, [this] (const QString& dirName) {
            ui->tw_rightPanel->setTabText(ui->tw_rightPanel->currentIndex(), dirName);
        });
    }

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
