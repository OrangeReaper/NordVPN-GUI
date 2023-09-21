#include "qgroupscommand.h"

QGroupsCommand::QGroupsCommand(QCallback *callback, QObject *parent) : QVPNCommand("nordvpn groups", 5000, callback, parent)
{
}
void QGroupsCommand::handleResponse(){
    QString _response=response();
    removeSpurii(_response);
    m_groups.clear();
    _response.replace(" ",""); // remove spaces
    if (_response.length() > 0){
        QStringList list = _response.split(',');
        m_groups.append(list);
        emit groupsUpdated(m_groups);
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
