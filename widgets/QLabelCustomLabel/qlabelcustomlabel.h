#ifndef QLABELCUSTOMLABEL_H
#define QLABELCUSTOMLABEL_H

#include <QLabel>
#include "widgets/QCustomLabel/qcustomlabel.h"

class QLabelCustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QLabelCustomLabel(QWidget *parent = nullptr);
    ~QLabelCustomLabel();

    void resizeEvent(QResizeEvent *event) override;
    QCustomLabel *addLabel();

    void setHighlightOnHover(bool highlight) {m_highlightOnHover = highlight;}

signals:
    void buttonClicked();

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void resizeLabels();

    const int y_tol = 1;
    const int x_tol = 0;

    QList<QCustomLabel*> m_labels;

    bool m_highlightOnHover = false;

};

#endif // QLABELCUSTOMLABEL_H

