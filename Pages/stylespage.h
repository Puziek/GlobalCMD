#ifndef STYLESPAGE_H
#define STYLESPAGE_H

#include "settingsmanager.h"
#include <QWidget>
#include <QDir>
#include <QComboBox>

namespace Ui {
class StylesPage;
}

class StylesPage : public QWidget
{
    Q_OBJECT

public:
    explicit StylesPage(QWidget *parent = 0);
    ~StylesPage();

    QString getSelectedStyle();

signals:
    void styleChanged(const QString&);

private:
    Ui::StylesPage *ui;
};

#endif // STYLESPAGE_H
