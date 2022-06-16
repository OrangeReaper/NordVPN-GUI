#include "qlogger.h"

#include <QDebug>

QLogger* QLogger::s_instance = 0;

QLogger* QLogger::getInstance(){
    if(!s_instance) {
        s_instance = new QLogger();
        // There is no instance so we created one.
        return s_instance;
    } else {
        //This is the same instance!
        return s_instance;
    }
}
QLogger::~QLogger(){
}

void QLogger::logSomething(QString str, QString color){
    qDebug() << str;
    emit log(str, color);
}
