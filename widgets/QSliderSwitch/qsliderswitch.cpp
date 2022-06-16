#include "qsliderswitch.h"
#include <QCoreApplication>
#include <QPainter>
#include <QTime>
#include <QPainterPath>
//
// Original code @ https://stackoverflow.com/questions/14780517/toggle-switch-in-qt/38102598
// Although more than 90% of this class has been re-written
//

QSliderSwitch::QSliderSwitch(QWidget* parent) : QWidget(parent)
{
  // This order for definition of widgets is important (these widgets overlap)
  m_background = new QWidget(this);
  m_label = new QLabel(this);
  m_circle = new SwitchCircle(this, m_offColor);

  m_btnMove = new QPropertyAnimation(this);
  m_backMove = new QPropertyAnimation(this);

  m_btnMove->setTargetObject(m_circle);
  m_btnMove->setPropertyName("pos");
  m_backMove->setTargetObject(m_background);
  m_backMove->setPropertyName("size");

  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);
  setContentsMargins(0,0,0,0);
  resize(QSize(m_defaultWidth, 24));
  setFixedHeight(24);
  m_background->resize(20, 20);
  m_background->move(2, 2);

  setValue(m_value);
  setEnabled(m_enabled);
  showValue();

  m_circle->move(2, 2);
  m_label->move(m_circle->width() + m_tol*3, 3);
  setLabelWidth();

}

QSliderSwitch::~QSliderSwitch(){
  delete m_circle;
  delete m_background;
  delete m_label;
  delete m_btnMove;
  delete m_backMove;
}

void QSliderSwitch::paintEvent(QPaintEvent*){
    QPainter* painter = new QPainter;
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::NoPen);
    painter->setPen(pen);

    painter->setBrush(m_penColor);
    painter->drawRoundedRect(0, 0
      , width(), height()
      , 12, 12);

    // set widget colour
    if (m_enabled){
        if (!m_value){
          painter->setBrush(m_offColor);
        } else {
          painter->setBrush(m_onColor);
        }
    } else {
        painter->setBrush(QColor(230, 230, 230)); // if disabled
    }
    painter->drawRoundedRect(2, 2
    , width() - 4, height() - 4
    , 10, 10);

    painter->end();
}
void QSliderSwitch::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    repositionCircle();
    setLabelWidth();
}
void QSliderSwitch::doPaint(){
    repaint();
    parentWidget()->repaint();
}
void QSliderSwitch::mousePressEvent(QMouseEvent *event){
  if (!m_enabled) return;
  setValue(!m_value);
  emit valueChanged(m_value);
  animate();

}
void QSliderSwitch::animate(){
    m_btnMove->stop();
    m_backMove->stop();

    m_btnMove->setDuration(m_duration);
    m_backMove->setDuration(m_duration);

    int hback = 20;
    QSize initial_size(hback, hback);
    QSize final_size(width() - 2*m_tol, hback);

    int xi = 2;
    int y  = 2;
    int xf = width() - 22;

    if (!m_value){
      final_size = QSize(hback, hback);
      initial_size = QSize(width() - 2*m_tol, hback);

      xi = xf;
      xf = 2;
    }

    m_btnMove->setStartValue(QPoint(xi, y));
    m_btnMove->setEndValue(QPoint(xf, y));

    m_backMove->setStartValue(initial_size);
    m_backMove->setEndValue(final_size);

    m_btnMove->start();
    m_backMove->start();
}


void QSliderSwitch::setEnabled(bool flag){
  m_enabled = flag;
  m_circle->setEnabled(flag);
  m_background->setEnabled(flag);
  QWidget::setEnabled(flag);
  setTextColor();
}

void QSliderSwitch::setDuration(int time){
  m_duration = time;
}

void QSliderSwitch::setValue(bool flag)
{
  if (flag == m_value) return;
  m_value = flag;
  showValue();


}
void QSliderSwitch::setOnColor(QColor color){
    m_onColor = color;
    doPaint();
}
void QSliderSwitch::setOffColor(QColor color){
    m_offColor = color;
    doPaint();
}
void QSliderSwitch::setOnText(QString text){
    m_onText = text;
    refreshOnOffText();

}
void QSliderSwitch::setOffText(QString text){
    m_offText = text;
    refreshOnOffText();
}
void QSliderSwitch::setOnTextColor(QColor color){
    m_onTextColor=color;
    setTextColor();
}
void QSliderSwitch::setOffTextColor(QColor color){
    m_offTextColor=color;
    setTextColor();
}
void QSliderSwitch::refreshOnOffText(){
    if (!m_value){
        m_label->setText(m_offText);
    } else {
        m_label->setText(m_onText);
    }
}
void QSliderSwitch::setTextColor(){
    //Changes the TEXT COLOR
    QString style="color: rgb(RED, GREEN, BLUE); font-weight: bold;";
    QColor color(120, 120, 120); // default if disabled
    if (m_enabled){
        if (m_value){
            color = m_onTextColor;
        } else {
            color = m_offTextColor;
        }
    }
    style.replace("RED", QString::number(color.red()));
    style.replace("GREEN", QString::number(color.green()));
    style.replace("BLUE", QString::number(color.blue()));
    m_label->setStyleSheet(style);
}
void QSliderSwitch::setLabelWidth(){
    m_label->setFixedWidth(width()-m_circle->width()-m_tol*2); //was 65
}
void QSliderSwitch::showValue(){
    refreshOnOffText();
    repositionCircle();
    setTextColor();
}

void QSliderSwitch::repositionCircle(){
    //initialise background size when value=OFF
    // background is oblong in this instance
    QSize finalSize(width() - 2*m_tol, m_background->height());

    //initialise Circle position for value=OFF
    int y = m_tol;
    int x = m_tol; // on the LHS

    if (m_value){ // background is square in this instance
        finalSize = QSize(m_background->height(), m_background->height());
        x = width() - (m_circle->width() + m_tol); // on the RHS
    }

    m_circle->move(QPoint(x, y));
    m_background->resize(finalSize);
}

QSliderSwitch::SwitchCircle::SwitchCircle(QWidget* parent, QColor color, bool rect)
  : QWidget(parent)
  , m_rect(rect)
  , m_borderRadius(12)
  , m_color(color)
  , m_Pencolor(QColor(120, 120, 120))
{
  setFixedSize(20, 20);

  m_rg = QRadialGradient(static_cast<int>(width() / 2), static_cast<int>(height() / 2), 12);
  m_rg.setColorAt(0, QColor(255, 255, 255));
  m_rg.setColorAt(0.6, QColor(255, 255, 255));
  m_rg.setColorAt(1, QColor(205, 205, 205));

  m_lg = QLinearGradient(3, 18, 20, 4);
  m_lg.setColorAt(0, QColor(255, 255, 255));
  m_lg.setColorAt(0.55, QColor(230, 230, 230));
  m_lg.setColorAt(0.72, QColor(255, 255, 255));
  m_lg.setColorAt(1, QColor(255, 255, 255));

  m_lgDisabled = QLinearGradient(3, 18, 20, 4);
  m_lgDisabled.setColorAt(0, QColor(230, 230, 230));
  m_lgDisabled.setColorAt(0.55, QColor(210, 210, 210));
  m_lgDisabled.setColorAt(0.72, QColor(230, 230, 230));
  m_lgDisabled.setColorAt(1, QColor(230, 230, 230));

  m_enabled = true;
}
QSliderSwitch::SwitchCircle::~SwitchCircle()
{
}
void QSliderSwitch::SwitchCircle::paintEvent(QPaintEvent*)
{
  QPainter* painter = new QPainter;
  painter->begin(this);
  painter->setRenderHint(QPainter::Antialiasing, true);

  QPen pen(Qt::NoPen);
  painter->setPen(pen);
  painter->setBrush(m_Pencolor);

  painter->drawEllipse(0, 0, 20, 20);
  painter->setBrush(m_rg);
  painter->drawEllipse(1, 1, 18, 18);

  painter->setBrush(QColor(210, 210, 210));
  painter->drawEllipse(2, 2, 16, 16);

  if (m_enabled)
  {
    painter->setBrush(m_lg);
    painter->drawEllipse(3, 3, 14, 14);
  }
  else
  {
    painter->setBrush(m_lgDisabled);
    painter->drawEllipse(3, 3, 14, 14);
  }

  painter->end();
}

