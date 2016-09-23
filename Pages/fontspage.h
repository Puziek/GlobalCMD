#ifndef FONTSPAGE_H
#define FONTSPAGE_H

#include "settingswindow.h"
#include <QWidget>
#include <QFont>

namespace Ui {
class FontsPage;
}

class FontsPage : public QWidget
{
    Q_OBJECT

public:
    explicit FontsPage(QWidget* parent = 0);
    ~FontsPage();
    void setMainFont(const QFont& mainFont);
    void setListFont(const QFont& listFont);

private:
    Ui::FontsPage *ui;
};

#endif // FONTSPAGE_H
