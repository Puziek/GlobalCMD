#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <algorithm>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    ui->sw_pages->addWidget(new FontsPage(this));
    ui->sw_pages->addWidget(new ColumnsPage(this));

    ui->sw_pages->removeWidget(ui->page);
    ui->sw_pages->removeWidget(ui->page_2);

    connect(dynamic_cast<ColumnsPage*> (ui->sw_pages->widget(ColumnPage)), &ColumnsPage::checkboxChanged,
            this, &SettingsWindow::updateShowColumnsInfo);

    connect(ui->lw_options, &QListWidget::currentItemChanged, this, &SettingsWindow::changePage);
    connect(ui->pb_apply, &QPushButton::clicked, this, &SettingsWindow::saveSettings);

    connect(ui->pb_ok, &QPushButton::clicked, this, [this] {
        saveSettings();
        close();
    });

    connect(ui->pb_cancel, &QPushButton::clicked, this, [this] {
        close();
    });

    ui->lw_options->setCurrentRow(FontPage);

    configChanged = false;
    ui->pb_apply->setEnabled(configChanged);

    loadSettings();

    //TODO: It shouldn't be here
    for (int widget = 0; widget < ui->sw_pages->count(); ++widget) {
        if (ui->sw_pages->widget(widget)->objectName() == "FontsPage") {
            ui->lw_options->addItem(tr("Fonts"));
        }
        else if (ui->sw_pages->widget(widget)->objectName() == "ColumnsPage") {
            ui->lw_options->addItem(tr("Columns"));
        }
    }
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::saveSettings()
{
    if (configChanged) {
        SettingsManager::putSetting("Fonts", "Main font", mainFont);
        SettingsManager::putSetting("Fonts", "List font", listFont);
        SettingsManager::putSetting("Columns", "Hidden columns", hiddenColumns);

        emit settingsChanged();
        configChanged = false;
        ui->pb_apply->setEnabled(configChanged);
    }
}

void SettingsWindow::loadSettings()
{
    mainFont = qvariant_cast<QFont>
               (SettingsManager::getSetting("Fonts", "Main font", QFont()));
    listFont = qvariant_cast<QFont>
               (SettingsManager::getSetting("Fonts", "List font", QFont()));

    hiddenColumns = SettingsManager::getSetting("Columns", "Hidden columns", QBitArray(FileListModel::Count)).toBitArray();
    dynamic_cast<FontsPage*> (ui->sw_pages->widget(FontPage))->setMainFont(mainFont);
    dynamic_cast<FontsPage*> (ui->sw_pages->widget(FontPage))->setListFont(listFont);
    dynamic_cast<ColumnsPage*> (ui->sw_pages->widget(ColumnPage))->updateCheckBoxes();
}

void SettingsWindow::updateShowColumnsInfo(const QBitArray &checkBoxes)
{
    hiddenColumns = checkBoxes;
    configChanged = true;
    ui->pb_apply->setEnabled(configChanged);
}

void SettingsWindow::showListFontDialog()
{
    listFont = QFontDialog::getFont(&configChanged, listFont, this);
    dynamic_cast<FontsPage*> (ui->sw_pages->widget(FontPage))->setListFont(listFont);
    ui->pb_apply->setEnabled(configChanged);
}

void SettingsWindow::showMainFontDialog()
{
    mainFont = QFontDialog::getFont(&configChanged, mainFont, this);
    dynamic_cast<FontsPage*> (ui->sw_pages->widget(FontPage))->setMainFont(mainFont);
    ui->pb_apply->setEnabled(configChanged);
}

void SettingsWindow::changePage(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (!current)
        current = previous;

    ui->sw_pages->setCurrentIndex(ui->lw_options->row(current));
}
