#include "qcountriescommand.h"

QCountriesCommand::QCountriesCommand(QCallback *callback, QObject *parent) : QVPNCommand(callback, parent)
{
}
void QCountriesCommand::execute(QProcess *vpnProcess){
    QString command = "nordvpn countries";
    sendCommand(vpnProcess, command, 5000);
}
void QCountriesCommand::handleResponse(){
    QString _response=responseList().at(0);
    //removeSpurii(_response);
    m_countries.clear();
    _response.replace(" ",""); // remove spaces
    //_response.replace(commandComplete(),"");
    if (_response.length() > 0){
        QStringList list = _response.split(',');
        m_countries.append(list);
        emit countriesUpdated(m_countries);
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
