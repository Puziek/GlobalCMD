#ifndef GLOBALCMD_H
#define GLOBALCMD_H

#include "filelistmodel.h"
#include "filelistpanel.h"
#include <QMainWindow>
#include <QStorageInfo>
#include <QKeyEvent>
#include <QVector>
#include <QShortcut>

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

private:
    void updateCurrentTabs(QWidget *firstPanel, QWidget *secondPanel, bool isFirstFocused);

    FileListPanel* focusedPanel;
    Ui::GlobalCMD* ui;

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // GLOBALCMD_H
