#ifndef FILELISTPANEL_H
#define FILELISTPANEL_H

#include "filelistmodel.h"
#include <QWidget>
#include <QMainWindow>
#include <QStorageInfo>

namespace Ui {
class FileListPanel;
}

class FileListPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FileListPanel(QWidget *parent = 0);
    ~FileListPanel();

public slots:
    void changeDriveReq(int index = 0);
    void onDirectoryChanged(const QString& path);

private:
    Ui::FileListPanel *ui;
    FileListModel* fileListModel;
};

#endif // FILELISTPANEL_H
