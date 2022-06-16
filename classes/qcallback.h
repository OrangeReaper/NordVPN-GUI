#ifndef QCALLBACK_H
#define QCALLBACK_H

#include <QObject>

class QCallback : public QObject
{
    Q_OBJECT
public:
    explicit QCallback(QObject *parent = nullptr);
signals:
    void callback(QString response);
};

#endif // CLASSES\QCALLBACK_H
