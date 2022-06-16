#ifndef QCUSTOMLINEEDIT_H
#define QCUSTOMLINEEDIT_H

#include <QLineEdit>
#include "../QCustomLabel/qcustomlabel.h"

class QCustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QCustomLineEdit(QWidget *parent = nullptr);
    ~QCustomLineEdit();
    //-- QWidget methods
    void resizeEvent(QResizeEvent *event) override;
    QCustomLabel *addLabel();
public slots:

signals:
    void buttonClicked();


private:  
    void resizeLabels();

    const int y_tol = 3;
    const int x_tol = 0;

    QList<QCustomLabel*> m_labels;
};

#endif // QCUSTOMLINEEDIT_H
