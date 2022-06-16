#include "qpasswordlineedit.h"
#include <QFontDatabase>
#include <QDebug>

QPasswordLineEdit::QPasswordLineEdit(QWidget *parent) : QCustomLineEdit(parent){
     m_label = addLabel();

    QFont fontAwesome;
    if (QFontDatabase::addApplicationFont(":/r/fontawesome-free-6.0.0-desktop/otfs/Font Awesome 6 Free-Regular-400.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    fontAwesome.setFamily("FontAwesome");
    m_label->setFont(fontAwesome);
    m_label->setAttribute(Qt::WA_TranslucentBackground);

    m_cursor = QCursor(Qt::PointingHandCursor);
    m_label->setCursor(m_cursor);

    showHide();
    connect(m_label, SIGNAL(clicked()), this, SLOT(showHide()));
}
QPasswordLineEdit::~QPasswordLineEdit() {}
void QPasswordLineEdit::showHide(){

    if (m_show){
        setEchoMode(QLineEdit::Normal);
        m_label->setText("\uf070");
    } else {
        setEchoMode(QLineEdit::Password);
        m_label->setText("\uf06e");
    }
    m_show = !m_show;
}
