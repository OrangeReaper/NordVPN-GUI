#include "qlabelcustomlabel.h"


QLabelCustomLabel::QLabelCustomLabel(QWidget *parent) : QLabel(parent){
    setContentsMargins(0,0,0,0);
}

QLabelCustomLabel::~QLabelCustomLabel() {}

QCustomLabel *QLabelCustomLabel::addLabel(){
    QCustomLabel *label = new QCustomLabel(this);
    label->setStyleSheet("QLabel { background-color : red; color : blue; }");
    m_labels.append(label);
    connect(label, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
    resizeLabels();
    return label;
}

void QLabelCustomLabel::resizeEvent(QResizeEvent *event){
    if (m_labels.size() == 0)
        return;

    resizeLabels();
    QLabel::resizeEvent(event);
}
void QLabelCustomLabel::resizeLabels(){
    QCustomLabel *label;
    for (int i = 0; i < m_labels.size(); i++) {
        label = m_labels.at(i);
        label->setGeometry( this->width() - ((i+1) * (this->height()) + x_tol), y_tol, this->height()-y_tol*2, this->height()-y_tol*2);
    }
}
void QLabelCustomLabel::enterEvent (QEvent * event){
  if (m_highlightOnHover)
      setStyleSheet("QLabel {background-color: rgb(206, 206, 206); }");

  QLabel::enterEvent(event);
};

void QLabelCustomLabel::leaveEvent(QEvent * event){
  setStyleSheet("");
  QLabel::leaveEvent(event);
}
