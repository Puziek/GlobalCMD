#ifndef GCMDTABLEVIEW_H
#define GCMDTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QKeyEvent>
#include <QDebug>

class GCMDTableView : public QTableView
{
    Q_OBJECT
public:
    explicit GCMDTableView(QWidget *parent = 0);

signals:
    void tabPressed();

public slots:

private:
    bool event(QEvent *event) override;
};

#endif // GCMDTABLEVIEW_H
