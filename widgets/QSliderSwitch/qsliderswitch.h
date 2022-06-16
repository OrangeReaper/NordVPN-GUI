#ifndef QSLIDERSWITCH_H
#define QSLIDERSWITCH_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

class QSliderSwitch : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(QSliderSwitch)

public:
    explicit QSliderSwitch(QWidget *parent = nullptr);
    ~QSliderSwitch() override;

      //-- QWidget methods
      void mousePressEvent(QMouseEvent *event) override;
      void paintEvent(QPaintEvent *event) override;
      void resizeEvent(QResizeEvent *event) override;
      void setEnabled(bool);

      //-- Setters
      void setDuration(int);
      void setValue(bool);
      void setLabelWidth();
      void setOnColor(QColor color);
      void setOffColor(QColor color);
      void setOnText(QString text);
      void setOffText(QString text);
      void setOnTextColor(QColor color);
      void setOffTextColor(QColor color);

      //-- Getters
      bool    value()        const {return m_value;}
      QColor  onColor()      const {return m_onColor;}
      QColor  offColor()     const {return m_offColor;}
      QColor  onTextColor()  const {return m_onTextColor;}
      QColor  offTextColor() const {return m_offTextColor;}
      QString onText()       const {return m_onText;}
      QString offText()      const {return m_offText;}
      int     duration()     const {return m_duration;}

    signals:
      void valueChanged(bool newvalue);

    private:
      class SwitchCircle;

      void repositionCircle();
      void animate();
      void showValue();
      void doPaint();
      void setTextColor();
      void refreshOnOffText();

    private:
      bool m_value=false;

      QColor m_penColor     = QColor(120, 120, 120); // changes outer border
      QColor m_offColor     = QColor(255, 0, 0);
      QColor m_onColor      = QColor(154, 205, 50);
      QColor m_offTextColor = QColor(255, 255, 255);
      QColor m_onTextColor  = QColor(255, 255, 255);

      QString m_onText   = "Enabled";
      QString m_offText  = "Disabled";
      int m_defaultWidth = 110;

      int m_tol             = 2; // space to leave around the border
      int m_borderRadius    = 12;
      int m_duration        = 100;

      QWidget*          m_background;
      QLabel*           m_label;
      SwitchCircle*     m_circle;

      bool m_enabled=true;

      QPropertyAnimation* m_btnMove;
      QPropertyAnimation* m_backMove;


};

class QSliderSwitch::SwitchCircle : public QWidget
{
  Q_OBJECT
    Q_DISABLE_COPY(SwitchCircle)

public:
  explicit SwitchCircle(QWidget* parent = nullptr, QColor color = QColor(255, 255, 255), bool rect = false);
  ~SwitchCircle() override;

  //-- QWidget methods
  void paintEvent(QPaintEvent* event) override;
  void setEnabled(bool enabled) {m_enabled = enabled;}

private:
  bool            m_rect;
  int             m_borderRadius;
  QColor          m_color;
  QColor          m_Pencolor;
  QRadialGradient m_rg;
  QLinearGradient m_lg;
  QLinearGradient m_lgDisabled;

  bool m_enabled;
};

#endif // QSLIDERSWITCH_H
