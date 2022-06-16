#include "qgroupscommand.h"

QGroupsCommand::QGroupsCommand(QCallback *callback, QObject *parent) : QVPNCommand(callback, parent)
{
}


void QGroupsCommand::execute(QProcess *vpnProcess){
    QString command = "nordvpn groups";
    sendCommand(vpnProcess, command, 5000);
}
void QGroupsCommand::handleResponse(){
    QString _response=responseList().at(0);
    //removeSpurii(_response);
    m_groups.clear();
    _response.replace(" ",""); // remove spaces
    //_response.replace(commandComplete(),"");
    if (_response.length() > 0){
        QStringList list = _response.split(',');
        m_groups.append(list);
        emit groupsUpdated(m_groups);
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
