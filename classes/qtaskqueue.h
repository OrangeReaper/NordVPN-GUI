#ifndef QTASKQUEUE_H
#define QTASKQUEUE_H

#include <QObject>
#include <QQueue>
#include "commands/qvpncommand.h"

class QTaskQueue : public QObject
{
    Q_OBJECT
public:
    virtual ~QTaskQueue();

    static QTaskQueue *getInstance();
    void enqueue(QVPNCommand *task);
    void clear();
    QVPNCommand* dequeue();

signals:
    void service();
public slots:

private:
    QTaskQueue(): QObject(nullptr) {} // Singleton Object
    static QTaskQueue *s_instance;

    QQueue<QVPNCommand*> m_queue;
};

#endif // QTASKQUEUE_H



