#include "qlogincommand.h"
#include <QSettings>

QLoginCommand::QLoginCommand(QCallback *callback, QObject *parent) : QVPNCommand("dummy", 10000, callback, parent)
{

}
void QLoginCommand::sendCommand(){
    QSettings settings;
/*    QString command = "nordvpn login --legacy --username "
                + settings.value("username").toString()
                + " --password "
                + settings.value("password").toString(); */


    QString command = "nordvpn login --token "
            + settings.value("password").toString();
    setCommand(command);
    QVPNCommand::sendCommand();
}
void QLoginCommand::handleResponse(){
    // Here when response arrives after login request
    if (response().contains("already logged in") || response().contains("Welcome to NordVPN")) {
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}

