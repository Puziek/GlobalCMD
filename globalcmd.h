#ifndef GLOBALCMD_H
#define GLOBALCMD_H

#include <QMainWindow>

namespace Ui {
class GlobalCMD;
}

class GlobalCMD : public QMainWindow
{
    Q_OBJECT

public:
    explicit GlobalCMD(QWidget *parent = 0);
    ~GlobalCMD();

private:
    Ui::GlobalCMD *ui;
};

#endif // GLOBALCMD_H
