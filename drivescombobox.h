#ifndef DRIVESCOMBOBOX_H
#define DRIVESCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include <QDebug>
#include <QFileIconProvider>

class DrivesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DrivesComboBox(QWidget *parent = 0);
    void showPopup() override;

signals:
    void clicked();

public slots:

private:

};

#endif // DRIVESCOMBOBOX_H
