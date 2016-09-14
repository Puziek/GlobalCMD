#ifndef DRIVESCOMBOBOX_H
#define DRIVESCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include <QDebug>

class DrivesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DrivesComboBox(QWidget *parent = 0);
    void showPopup() override;

signals:
    void clicked();

public slots:
};

#endif // DRIVESCOMBOBOX_H
