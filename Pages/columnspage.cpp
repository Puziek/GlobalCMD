#include "columnspage.h"
#include "ui_columnspage.h"

ColumnsPage::ColumnsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColumnsPage)
{
    ui->setupUi(this);

    //TODO: Check why this part causes "QObject::connect: invalid null parameter" error
    for (QObject* obj : ui->gb_showColumns->children()) {
        connect(dynamic_cast<QCheckBox*> (obj), &QCheckBox::clicked, this, [this] {
            emit checkboxChanged(getCheckboxesState());
        });
    }
}

ColumnsPage::~ColumnsPage()
{
    delete ui;
}

void ColumnsPage::updateCheckBoxes()
{
    QBitArray checkBoxes = SettingsManager::getSetting("Columns", "Hidden columns", QBitArray(FileListModel::Count)).toBitArray();

    ui->cb_showFilename->setChecked(checkBoxes.at(FileListModel::FileName));
    ui->cb_showExtension->setChecked(checkBoxes.at(FileListModel::Suffix));
    ui->cb_showSize->setChecked(checkBoxes.at(FileListModel::Size));
    ui->cb_showTime->setChecked(checkBoxes.at(FileListModel::Date));
    ui->cb_showAttr->setChecked(checkBoxes.at(FileListModel::Attributes));
}

QBitArray ColumnsPage::getCheckboxesState()
{
    QBitArray checkBoxes(FileListModel::Count);

    checkBoxes.setBit(FileListModel::FileName, ui->cb_showFilename->isChecked());
    checkBoxes.setBit(FileListModel::Suffix, ui->cb_showExtension->isChecked());
    checkBoxes.setBit(FileListModel::Size, ui->cb_showSize->isChecked());
    checkBoxes.setBit(FileListModel::Date, ui->cb_showTime->isChecked());
    checkBoxes.setBit(FileListModel::Attributes, ui->cb_showAttr->isChecked());

    return checkBoxes;
}
