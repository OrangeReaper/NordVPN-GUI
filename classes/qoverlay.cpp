#include "qoverlay.h"

#include <QPropertyAnimation>
#include <QSpacerItem>
#include <QDebug>

#include "classes/staticfunctions.h"

QOverlay::QOverlay(QWidget *parent) :
    QWidget(parent)
{
    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_textEdit = new QTextEdit();
    this->setStyleSheet("QTextEdit {color: yellow; background-color: black; }");
    layout()->addWidget(m_textEdit);
    m_textEdit->setAcceptRichText(true);
    m_textEdit->setReadOnly(true);

    QFont font=m_textEdit->currentFont();
    font.setPointSize(10);
    m_textEdit->setCurrentFont(font);

    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->setContentsMargins(0,0,0,0);
    m_buttonLayout->setSpacing(0);
    m_mainLayout->addLayout(m_buttonLayout);

    m_button = new QCustomLabel();
    connect(m_button, SIGNAL(clicked()), this, SLOT(expandOrContract()));
    m_button->setFixedSize(c_buttonWidth,c_buttonHeight);
    m_button->setAlignment(Qt::AlignmentFlag::AlignTop);
    m_button->setPixmap(QPixmap(":/r/images/mytab.png").scaled(c_buttonWidth,c_buttonHeight,Qt::KeepAspectRatio));

    m_buttonLayout->addItem(new QSpacerItem(1000, 1,  QSizePolicy::Fixed, QSizePolicy::Expanding));
    m_buttonLayout->addWidget(m_button);
    m_buttonLayout->addItem(new QSpacerItem(1000, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));


}
QOverlay::~QOverlay(){
}
void QOverlay::contract(){
    if (m_textEdit->height() == 0)
        return;

    expandOrContract();
}

void QOverlay::logSomething(QString text, QString color){
    m_textEdit->setTextColor(QColor(color));
    m_textEdit->append(text);
    emit logged(text);
}
void QOverlay::expandOrContract(){
    QPropertyAnimation *animation = new QPropertyAnimation(m_textEdit, "size");
    animation->setDuration(m_animationSpeed);
    QSize startTextEditSize=m_textEdit->size();
    animation->setStartValue(startTextEditSize);

    QPropertyAnimation *animation2 = new QPropertyAnimation(m_button, "geometry");
    animation2->setDuration(m_animationSpeed);
    QRect buttonGeometry = m_button->geometry();
    animation2->setStartValue(buttonGeometry);

    int newHeight = c_buttonHeight;

    if (m_textEdit->height() != 0){
        //contract
        //Finish textEdit size has height =0
        animation->setEndValue(QSize(m_textEdit->width(), 0));
        QRect finalButtonGeometry(buttonGeometry.left(), 0, c_buttonWidth, c_buttonHeight);
        animation2->setEndValue(finalButtonGeometry);
        //this->setFixedHeight(c_buttonHeight);
    } else {
        //expand
        animation->setEndValue(QSize(m_textEdit->width(), m_maxHeight));
        QRect finalButtonGeometry(buttonGeometry.left(), m_maxHeight, c_buttonWidth, c_buttonHeight);
        animation2->setEndValue(finalButtonGeometry);
        newHeight = m_maxHeight + c_buttonHeight;
        this->setFixedHeight(newHeight);
    }
    animation->start();
    animation2->start();
    staticFunctions::delay(m_animationSpeed);
    this->setFixedHeight(newHeight);

}
