#include "qcountriescommand.h"

QCountriesCommand::QCountriesCommand(QCallback *callback, QObject *parent) : QVPNCommand("nordvpn countries", 5000, callback, parent)
{
}
void QCountriesCommand::handleResponse(){
    QString _response=response();
    removeSpurii(_response);
    removeLeadingSpurii(_response);
    m_countries.clear();
    _response.replace(" ",""); // remove spaces

    if (_response.length() > 0){
        QStringList list = _response.split(',');
        m_countries.append(list);
        emit countriesUpdated(m_countries);
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
