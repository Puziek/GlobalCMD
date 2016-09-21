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

void DrivesComboBox::updateDrivesList(QDir currDir)
{
    QFileIconProvider iconProvider;

    for (const QFileInfo& drive : QDir::drives()) {
        QComboBox::addItem(drive.filePath());
        setItemIcon(count() - 1, iconProvider.icon(QFileIconProvider::Drive));
    }

    while (currDir.cdUp());

    for (int item = 0; item < QComboBox::count(); ++item) {
        if (QComboBox::itemText(item) == currDir.absolutePath()) {
            QComboBox::setCurrentIndex(item);
        }
    }
}

