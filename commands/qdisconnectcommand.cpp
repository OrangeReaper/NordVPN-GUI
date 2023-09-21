#include "qdisconnectcommand.h"
#include <QSettings>

QDisconnectCommand::QDisconnectCommand(QCallback *callback, QObject *parent) : QVPNCommand("nordvpn disconnect", 20000, callback, parent)
{

}
void QDisconnectCommand::handleResponse(){
    // Here when response arrives after disconnect request
    if (response().contains("You are disconnected") || response().contains("You are not connected")) {
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
