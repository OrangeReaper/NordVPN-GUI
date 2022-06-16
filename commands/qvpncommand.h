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
    explicit QVPNCommand(QCallback *callback, QObject *parent = nullptr);
    ~QVPNCommand();
    virtual void handleTimeout();
    virtual void handleResponse();
    virtual void execute (QProcess *vpnProcess);
    void sendCommand(QProcess *vpnProcess, QString command, int timeout);
    void cancelTimeout();
    void commandSucceeded();
    void commandFailed();

    void setLogging(bool enabled) {m_logging = enabled;}
    void removeSpurii(QString &response);
    void removeLeadingSpurii(QString &str);
    QString parseResponse(QString find, QString response);

    QString commandComplete() {return c_commandComplete;}
    QString response() {return m_response;}
    QStringList responseList() {return m_responseList;}

    QCallback *callback() {return m_callback;}
public slots:
    void timedOut();
    void getResponse();
signals:
    void log(QString str, QString color);
    void success();
    void failed(QString message);

private:
    void logSomething(QString str, QString color);

    void finished();
    bool m_logging=true;

    QString m_lastCommand;

    QTimer * m_timer;
    QString m_response="";
    QStringList m_responseList;
    QProcess *m_vpnProcess;
    QCallback *m_callback=0;

    // Constants
    const QString c_commandComplete="@Done@";
    const QString c_commandColor="#ffffff"; // White
    const QString c_responseColor="#ffff33"; // Yellow
};

#endif // QVPNCOMMAND_H
