#ifndef QMANAGEDITEM_H
#define QMANAGEDITEM_H

#include "widgets/QLabelCustomLabel/qlabelcustomlabel.h"

class QManagedItem : public QLabelCustomLabel
{
    Q_OBJECT
public:
    explicit QManagedItem(QWidget *parent = nullptr);
    ~QManagedItem();

signals:


private:
    QCustomLabel *m_label;
    QCursor m_cursor;
};

#endif // QMANAGEDITEM_H
