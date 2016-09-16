#include "gcmdtableview.h"

GCMDTableView::GCMDTableView(QWidget *parent) : QTableView(parent)
{
}

bool GCMDTableView::event(QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Tab) {
            emit tabPressed();
            return true;
        }
    }

    return QTableView::event(event);
}

