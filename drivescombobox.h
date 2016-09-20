#ifndef DRIVESCOMBOBOX_H
#define DRIVESCOMBOBOX_H

#include <QObject>
#include <QDir>
#include <QComboBox>
#include <QDebug>
#include <QFileIconProvider>

class DrivesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DrivesComboBox(QWidget *parent = 0);
    void showPopup() override;

    void updateDrivesList();

public slots:

private:
    QStringList listOfDrives;

};

#endif // DRIVESCOMBOBOX_H
