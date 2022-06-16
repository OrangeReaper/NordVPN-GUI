#ifndef QCUSTOMLABEL_H
#define QCUSTOMLABEL_H

#include <QLabel>

class QCustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QCustomLabel(QWidget *parent = nullptr);
    ~QCustomLabel();
    void setCursor(QCursor &cursor);
    void overrideCursor(bool override) { m_overrideCursor = override;}
    bool cursorIsOverriden() { return m_overrideCursor; }
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QCursor m_cursor;
    bool m_overrideCursor=false;
};

#endif // QCUSTOMLABEL_H
