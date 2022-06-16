#include "qanycommand.h"

QAnyCommand::QAnyCommand(QCallback *callback, QString command, int msTimeout, QObject *parent) : QVPNCommand(callback, parent){
    m_command=command;
    m_timeout=msTimeout;

}
void QAnyCommand::execute(QProcess *vpnProcess){
    sendCommand(vpnProcess, m_command, m_timeout);
}
