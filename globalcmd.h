#ifndef GLOBALCMD_H
#define GLOBALCMD_H

#include "filelistmodel.h"
#include <QMainWindow>
#include <QStorageInfo>

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

private:
    bool isLeftPanelFocused;
    Ui::GlobalCMD* ui;
};

#endif // GLOBALCMD_H
