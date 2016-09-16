#include "drivescombobox.h"

DrivesComboBox::DrivesComboBox(QWidget *parent) : QComboBox(parent)
{
}

void DrivesComboBox::showPopup()
{
    QFileIconProvider iconProvider;

    emit clicked();
    for (int item = 0; item < count(); ++item) {
        setItemIcon(item, iconProvider.icon(QFileIconProvider::Drive));
    }
    QComboBox::showPopup();
}

