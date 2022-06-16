#ifndef QLOGGER_H
#define QLOGGER_H

#include <QObject>

class QLogger : public QObject
{
    Q_OBJECT
public:
//    explicit QLogger(QObject *parent = nullptr);
    static QLogger *getInstance();
    ~QLogger();
    void setTarget(QObject *target);
public slots:
    void logSomething(QString str, QString color);
signals:
    void log(QString str, QString color);
private:
    QLogger(){} // Singleton Object
    static QLogger *s_instance;

};

#endif // QLOGGER_H
