#include "qlabelswitch.h"
#include "QHBoxLayout"

QLabelSwitch::QLabelSwitch(QWidget *parent): QWidget{parent}{
    QHBoxLayout *l_layout = new QHBoxLayout();
    setLayout(l_layout);

    m_label = new QLabel(this);
    m_label->setFixedWidth(110);
    m_label->setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignRight);
    l_layout->addWidget(m_label);


    m_sliderSwitch = new QSliderSwitch(this);
    m_sliderSwitch->setFixedWidth(110);
    l_layout->addWidget(m_sliderSwitch);
    l_layout->setContentsMargins(0,0,0,0);

    l_layout->addStretch();
}
