#include "qcustomlineedit.h"

QCustomLineEdit::QCustomLineEdit(QWidget *parent) : QLineEdit(parent){
    setContentsMargins(0,0,0,0);
}
QCustomLineEdit::~QCustomLineEdit() {}

void QCustomLineEdit::resizeEvent(QResizeEvent *event){
    if (m_labels.size() == 0)
        return;

    resizeLabels();
    QLineEdit::resizeEvent(event);
}
void QCustomLineEdit::resizeLabels(){
    QCustomLabel *label;
    for (int i = 0; i < m_labels.size(); i++) {
        label = m_labels.at(i);
        label->setGeometry( this->width() - ((i+1) * (this->height()) + x_tol), y_tol, this->height()-y_tol*2, this->height()-y_tol*2);
    }
}
QCustomLabel *QCustomLineEdit::addLabel(){
    QCustomLabel *label = new QCustomLabel(this);
    label->setStyleSheet("QLabel { background-color : red; color : blue; }");
    m_labels.append(label);
    connect(label, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
    resizeLabels();
    return label;
}
