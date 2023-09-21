#include "qlogoutommand.h"

QLogoutCommand::QLogoutCommand(QCallback *callback, QObject *parent) : QVPNCommand("nordvpn logout", 3000, callback, parent)
{

}

void QLogoutCommand::handleResponse(){
    // Here when response arrives after logout request
    if (response().contains("logged out") || response().contains("not logged in")) {
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
