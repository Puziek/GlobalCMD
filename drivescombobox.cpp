#include "drivescombobox.h"

DrivesComboBox::DrivesComboBox(QWidget *parent) : QComboBox(parent)
{

}

void DrivesComboBox::showPopup()
{
    QComboBox::showPopup();
    emit clicked();
    qDebug() << "TEST";
}
