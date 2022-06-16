#ifndef QPASSWORDLINEEDIT_H
#define QPASSWORDLINEEDIT_H

#include "widgets/QCustomLineEdit/qcustomlineedit.h"

class QPasswordLineEdit : public QCustomLineEdit
{
    Q_OBJECT
public:
    explicit QPasswordLineEdit(QWidget *parent = nullptr);
    ~QPasswordLineEdit();
public slots:
    void showHide();
signals:

private:
    QCustomLabel *m_label;

    QCursor m_cursor;
    bool m_show=false; // initial state is hidden;
};

#endif // QPASSWORDLINEEDIT_H
