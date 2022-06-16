#include "qtaskqueue.h"
#include <QMutexLocker>

QTaskQueue* QTaskQueue::s_instance = 0;

QTaskQueue* QTaskQueue::getInstance(){
    if (!s_instance) {
        // There is no instance so we create one
        s_instance = new QTaskQueue();
    }
    return s_instance;
}
QTaskQueue::~QTaskQueue(){
}
void QTaskQueue::enqueue(QVPNCommand *task){
    m_queue.enqueue(task);
    emit service();
}
QVPNCommand* QTaskQueue::dequeue(){
    if (m_queue.length() >0){
        return m_queue.dequeue();
    } else {
        return 0;
    }
}
void QTaskQueue::clear(){
    if (m_queue.length() >0){
        m_queue.clear();
    }
}
