#include "qlogincommand.h"
#include <QSettings>

QLoginCommand::QLoginCommand(QCallback *callback, QObject *parent) : QVPNCommand(callback, parent)
{

}
void QLoginCommand::execute(QProcess *vpnProcess){
    QSettings settings;
    QString command = "nordvpn login --legacy --username "
                + settings.value("username").toString()
                + " --password "
                + settings.value("password").toString();
    sendCommand(vpnProcess, command, 10000);
}
void QLoginCommand::handleResponse(){
    // Here when response arrives after login request
    if (response().contains("already logged in") || response().contains("Welcome to NordVPN")) {
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}

