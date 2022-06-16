#include "qdisconnectcommand.h"
#include <QSettings>

QDisconnectCommand::QDisconnectCommand(QCallback *callback, QObject *parent) : QVPNCommand(callback, parent)
{

}
void QDisconnectCommand::execute(QProcess *vpnProcess){
    QSettings settings;
    QString command = "nordvpn disconnect";
    sendCommand(vpnProcess, command, 20000);
}
void QDisconnectCommand::handleResponse(){
    // Here when response arrives after disconnect request
    if (response().contains("You are disconnected") || response().contains("You are not connected")) {
        commandSucceeded();
        //setState(Connected);
    }
    QVPNCommand::handleResponse();
}
