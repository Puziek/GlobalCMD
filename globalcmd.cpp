#include "globalcmd.h"
#include "ui_globalcmd.h"

GlobalCMD::GlobalCMD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlobalCMD)
{
    ui->setupUi(this);

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

    connect(ui->actionSettings, &QAction::triggered, this, [this] {
        settingsWindow = new SettingsWindow();
        settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(settingsWindow, &SettingsWindow::settingsChanged, this, &GlobalCMD::updateConfig);
        settingsWindow->exec();
    });

    connect(ui->pb_copy, &QPushButton::released, this, [this] {
        focusedPanel->copyFiles();
    });

    connect(ui->pb_move, &QPushButton::released, this, [this] {
        focusedPanel->moveFiles();
    });

    connect(ui->pb_makeDir, &QPushButton::released, this, [this] {
        focusedPanel->createDirectory();
    });

    connect(ui->pb_remove, &QPushButton::released, this, [this] {
        focusedPanel->removeFiles();
    });

    updateConfig();
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
        ui->tw_leftPanel->widget(ui->tw_leftPanel->currentIndex())->setAttribute(Qt::WA_DeleteOnClose);
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
        ui->tw_rightPanel->widget(ui->tw_rightPanel->currentIndex())->setAttribute(Qt::WA_DeleteOnClose);
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

void GlobalCMD::updateConfig()
{
    FileListPanel* panel;
    for (int widget = 0; widget < ui->tw_leftPanel->count(); ++widget) {
        panel = dynamic_cast<FileListPanel*> (ui->tw_leftPanel->widget(widget));
        panel->updateFont(qvariant_cast<QFont>(SettingsManager::getSetting("Fonts", "List font", QFont())));
        panel->setHiddenColumns(SettingsManager::getSetting("Columns", "Hidden columns",
                                                            QBitArray(FileListModel::Count)).toBitArray());
    }

    for (int widget = 0; widget < ui->tw_rightPanel->count(); ++widget) {
        panel = dynamic_cast<FileListPanel*> (ui->tw_rightPanel->widget(widget));
        panel->updateFont(qvariant_cast<QFont>(SettingsManager::getSetting("Fonts", "List font", QFont())));
        panel->setHiddenColumns(SettingsManager::getSetting("Columns", "Hidden columns",
                                                            QBitArray(FileListModel::Count)).toBitArray());
    }

    setFont(qvariant_cast<QFont>
           (SettingsManager::getSetting("Fonts", "Main font", QFont())));
    ui->tw_leftPanel->setFont(qvariant_cast<QFont>
                             (SettingsManager::getSetting("Fonts", "Main font", QFont())));
    ui->tw_rightPanel->setFont(qvariant_cast<QFont>
                              (SettingsManager::getSetting("Fonts", "Main font", QFont())));
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

    case Qt::Key_F8:
        focusedPanel->removeFiles();

    case Qt::Key_F5:
        focusedPanel->copyFiles();
        return;

    case Qt::Key_F6:
        focusedPanel->moveFiles();
        return;

    case Qt::Key_F7:
        focusedPanel->createDirectory();
        return;

    case Qt::Key_T:
        if(event->modifiers() & Qt::ControlModifier) {
            createNewTab();
        };
        return;

    case Qt::Key_W:
        if(event->modifiers() & Qt::ControlModifier) {
            removeCurrentTab();
        };
        return;
    }

    QWidget::keyPressEvent(event);
}
