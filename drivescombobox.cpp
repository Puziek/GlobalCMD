#include "drivescombobox.h"

DrivesComboBox::DrivesComboBox(QWidget *parent) : QComboBox(parent)
{

    updateDrivesList();
}

void DrivesComboBox::showPopup()
{
    clear();
    updateDrivesList();
    QComboBox::showPopup();
}

void DrivesComboBox::updateDrivesList()
{
    QFileIconProvider iconProvider;

    for (const QFileInfo& drive : QDir::drives()) {
        QComboBox::addItem(drive.filePath());
        setItemIcon(count() - 1, iconProvider.icon(QFileIconProvider::Drive));
    }
}

