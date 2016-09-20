#ifndef FILELISTPANEL_H
#define FILELISTPANEL_H

#include "filelistmodel.h"
#include "fileoperator.h"
#include <QWidget>
#include <QMainWindow>
#include <QStorageInfo>
#include <QKeyEvent>
#include <QInputDialog>
#include <QMessageBox>

namespace Ui {
class FileListPanel;
}

class FileListPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FileListPanel(QWidget* parent = 0);
    ~FileListPanel();

    void copyFiles();
    void removeFiles();
    void moveFiles();
    void createDirectory();
    void goDirUp();
    void setBuddyPanel(FileListPanel* buddy);
    bool checkMembership();

    QDir buddyPath;

signals:
    void directoryChanged(const QString& path);
    void focusChanged(FileListPanel*);

public slots:
    void changeDriveReq(int index = 0);
    void setDirectoryPath(const QString& path);
    void setDirectory(const QModelIndex& index);


private:
    Ui::FileListPanel *ui;
    FileListModel* fileListModel;
    FileListPanel* buddyPanel;

    bool isInLeftTabs;
};

#endif // FILELISTPANEL_H
