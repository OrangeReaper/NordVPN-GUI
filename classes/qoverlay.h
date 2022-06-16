#ifndef QOVERLAY_H
#define QOVERLAY_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "widgets/QCustomLabel/qcustomlabel.h"

class QOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit QOverlay(QWidget *parent = nullptr);
    ~QOverlay();
    void contract();
    void setAnimationSpeed(int speedms) { m_animationSpeed = speedms; }
    void setMaxHeight(int height) { m_maxHeight = height; }
public slots:
    void expandOrContract();
    void logSomething(QString text, QString color);
signals:
    void    reposition();
    void    logged(QString text);
private:
    //properties
    int  m_maxHeight        = 300;
    int  m_animationSpeed   = 500;

    QTextEdit *m_textEdit;
    QHBoxLayout *m_buttonLayout;
    QVBoxLayout *m_mainLayout;
    QCustomLabel *m_button;

    //constants
    int c_buttonWidth   = 70;
    int c_buttonHeight  = 20;

};

#endif // QOVERLAY_H
