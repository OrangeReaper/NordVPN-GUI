#ifndef QVPNCOMMAND_H
#define QVPNCOMMAND_H

#include <QObject>
#include <QProcess>
#include <QTimer>

#include "classes/qlogger.h"
#include "classes/qcallback.h"

class QVPNCommand : public QObject
{
    Q_OBJECT
public:
    explicit QVPNCommand(QString command, int timeout, QCallback *callback, QObject *parent = nullptr);
    ~QVPNCommand();
    virtual void handleTimeout();
    virtual void handleResponse();
    virtual void sendCommand();

    QString command() { return m_command; }
    int timeout() { return m_timeout; }

    void setCommand(QString command) { m_command=command;}
    void setTimeout( int msTimeout) {m_timeout=msTimeout;}

    void commandSucceeded();
    void commandFailed();

    void setLogging(bool enabled) {m_logging = enabled;}
    void removeSpurii(QString &response);
    void removeLeadingSpurii(QString &str);
    QString parseResponse(QString find, QString response);

    QString response() {return m_response;}

    QCallback *callback() {return m_callback;}
public slots:

signals:
    void log(QString str, QString color);
    void success();
    void failed(QString message);

private:
    void logSomething(QString str, QString color);

    void finished();
    bool m_logging=true;

    QString m_command;
    int m_timeout;
    QString m_lastCommand;

    QTimer * m_timer;
    QString m_response="";
    QProcess *m_vpnProcess;
    QCallback *m_callback=0;

    // Constants
    const QString c_commandColor="#ffffff"; // White
    const QString c_responseColor="#ffff33"; // Yellow
    const QString c_errorColor="#ff0000"; // Red
};

#endif // QVPNCOMMAND_H
