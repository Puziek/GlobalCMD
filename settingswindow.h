#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "settingsmanager.h"
#include "filelistmodel.h"
#include "Pages/fontspage.h"
#include "Pages/columnspage.h"
#include <QDialog>
#include <QListWidgetItem>
#include <QFontDialog>
#include <QDebug>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();
    void saveSettings();
    void loadSettings();

signals:
    void settingsChanged();

public slots:
    void updateShowColumnsInfo(const QBitArray& checkBoxes);
    void showMainFontDialog();
    void showListFontDialog();

private:

    enum Pages {
        FontPage,
        ColumnPage,
        LangPage,
        Count
    };

    void changePage(QListWidgetItem* current, QListWidgetItem* previous);

    Ui::SettingsWindow *ui;
    bool configChanged;

    QFont mainFont;
    QFont listFont;
    QBitArray hiddenColumns;
    QString currStyle;
};

#endif // SETTINGSWINDOW_H
