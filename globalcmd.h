#ifndef GLOBALCMD_H
#define GLOBALCMD_H

#include "filelistmodel.h"
#include <QMainWindow>

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
    void onDiskChange(int index);

private:
    Ui::GlobalCMD* ui;
    FileListModel* fileListModel;
};

#endif // GLOBALCMD_H
