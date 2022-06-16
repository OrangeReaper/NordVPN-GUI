#ifndef QLABELSWITCH_H
#define QLABELSWITCH_H

#include <QWidget>
#include "../QSliderSwitch/qsliderswitch.h"

class QLabelSwitch : public QWidget
{
    Q_OBJECT
public:
    explicit QLabelSwitch(QWidget *parent = nullptr);
    QSliderSwitch *sliderSwitch() {return m_sliderSwitch;}
    QLabel *label() {return m_label;}
signals:
private:
    QSliderSwitch *m_sliderSwitch;
    QLabel *m_label;
};

#endif // QLABELSWITCH_H
