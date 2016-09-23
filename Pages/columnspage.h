#ifndef COLUMNSPAGE_H
#define COLUMNSPAGE_H

#include "settingswindow.h"
#include "filelistmodel.h"
#include <QWidget>
#include <QBitArray>

namespace Ui {
class ColumnsPage;
}

class ColumnsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ColumnsPage(QWidget *parent = 0);
    ~ColumnsPage();

    void updateCheckBoxes();
    QBitArray getCheckboxesState();

signals:
    void checkboxChanged(const QBitArray& checkBoxes);

private:
    Ui::ColumnsPage *ui;
};

#endif // COLUMNSPAGE_H
