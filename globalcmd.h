#ifndef GLOBALCMD_H
#define GLOBALCMD_H

#include "filelistmodel.h"
#include "filelistpanel.h"
#include "settingswindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QStorageInfo>
#include <QKeyEvent>
#include <QVector>

namespace Ui {
class GlobalCMD;
}

class GlobalCMD : public QMainWindow
{
    Q_OBJECT

public:
    explicit GlobalCMD(QWidget *parent = 0);
    ~GlobalCMD();

public slots:
    void createNewTab();
    void removeCurrentTab();
    void updateConfig();

private:
    void updateCurrentTabs(QWidget *firstPanel, QWidget *secondPanel, bool isFirstFocused);
    void connectPanelSignals(FileListPanel* panel, bool isFirstFocused);

    FileListPanel* focusedPanel;
    SettingsWindow* settingsWindow;
    Ui::GlobalCMD* ui;

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // GLOBALCMD_H
