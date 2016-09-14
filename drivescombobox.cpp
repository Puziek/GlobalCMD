#include "drivescombobox.h"

DrivesComboBox::DrivesComboBox(QWidget *parent) : QComboBox(parent)
{

}

void DrivesComboBox::showPopup()
{
    emit clicked();
    QComboBox::showPopup();
}
