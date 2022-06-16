#include "qcustomlabel.h"
#include <QGuiApplication>

QCustomLabel::QCustomLabel(QWidget *parent) : QLabel(parent){

}
QCustomLabel::~QCustomLabel() {
}

void QCustomLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();

    QLabel::mousePressEvent(event);
}
void QCustomLabel::enterEvent (QEvent * event){
  if (m_overrideCursor)
      QGuiApplication::setOverrideCursor(m_cursor);

  QLabel::enterEvent(event);
};

void QCustomLabel::leaveEvent(QEvent * event){
  if (m_overrideCursor)
      QGuiApplication::restoreOverrideCursor();

  QLabel::leaveEvent(event);

}
void QCustomLabel::setCursor(QCursor &cursor){
    m_cursor = cursor;
    m_overrideCursor=true;
}
