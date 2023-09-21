#include "qanycommand.h"

QAnyCommand::QAnyCommand(QCallback *callback, QString command, int msTimeout, QObject *parent) : QVPNCommand(command, msTimeout, callback, parent){
    m_command=command;
    m_timeout=msTimeout;

}
