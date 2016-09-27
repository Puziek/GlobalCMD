#ifndef FILELISTPANEL_H
#define FILELISTPANEL_H

#include "filelistmodel.h"
#include "fileoperator.h"
#include "settingsmanager.h"
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
    explicit FileListPanel(QString startingPath = 0, QWidget* parent = 0);
    ~FileListPanel();

    void copyFiles();
    void removeFiles();
    void moveFiles();
    void createDirectory();
    void goDirUp();
    void setBuddyPanel(FileListPanel* buddy);
    void updateFont(const QFont& font);
    void setHiddenColumns(const QBitArray& hiddenColumns);
    void createNewTab();
    void removeCurrentTab();
    QString getCurrDirName();
    QString getCurrDir();

    QDir buddyPath;

signals:
    void dirNameChanged(const QString& dirName);
    void focusChanged(FileListPanel* panel);

public slots:
    void changeDriveReq(int index = 0);
    void setDirectoryPath(const QString& path);
    void setDirectory(const QModelIndex& index);


private:
    Ui::FileListPanel *ui;
    FileListModel* fileListModel;
    FileListPanel* buddyPanel;

};

#endif // FILELISTPANEL_H
