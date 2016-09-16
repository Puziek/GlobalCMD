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

    void copyFiles();
    void removeFiles();
    void setBuddyPanel(FileListPanel* buddy);

    QDir buddyPath;

signals:
    void directoryChanged(const QString& path);
    void focusChanged();

public slots:
    void changeDriveReq(int index = 0);
    void onDirectoryChanged(const QString& path);

private:
    Ui::FileListPanel *ui;
    FileListModel* fileListModel;
    FileListPanel* buddyPanel;
};

#endif // FILELISTPANEL_H
