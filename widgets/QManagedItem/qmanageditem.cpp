#include "qmanageditem.h"
#include <QFontDatabase>
#include <QDebug>

QManagedItem::QManagedItem(QWidget *parent) : QLabelCustomLabel(parent){
    m_label = addLabel();

    QFont fontAwesome;
    if (QFontDatabase::addApplicationFont(":/r/fontawesome-free-6.0.0-desktop/otfs/Font Awesome 6 Free-Regular-400.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    fontAwesome.setFamily("FontAwesome");
    m_label->setFont(fontAwesome);
    m_label->setAttribute(Qt::WA_TranslucentBackground);
    m_label->setText("\uf2ed");

    m_cursor = QCursor(Qt::PointingHandCursor);
    m_label->setCursor(m_cursor);

    setHighlightOnHover(true);
    connect(m_label, SIGNAL(clicked()), this, SLOT(deleteLater()));
}
QManagedItem::~QManagedItem() {}


