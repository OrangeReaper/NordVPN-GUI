#include "qlogoutommand.h"

QLogoutCommand::QLogoutCommand(QCallback *callback, QObject *parent) : QVPNCommand(callback, parent)
{

}
void QLogoutCommand::execute(QProcess *vpnProcess){
    QString command = "nordvpn logout; nordvpn account";
    sendCommand(vpnProcess, command, 3000);
}
void QLogoutCommand::handleResponse(){
    // Here when response arrives after logout request
    if (response().contains("logged out") || response().contains("not logged in")) {
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
