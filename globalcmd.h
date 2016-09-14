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
    void changeDriveReq(int index = 0);
    void onDirectoryChanged(const QString& path);

private:
    Ui::GlobalCMD* ui;
    FileListModel* fileListModel;
};

#endif // GLOBALCMD_H
