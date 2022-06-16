#include "qstatuscommand.h"

QStatusCommand::QStatusCommand(QCallback *callback, QObject *parent) : QVPNCommand(callback, parent)
{
    setLogging(false);
}
void QStatusCommand::execute(QProcess *vpnProcess){
    QString command = "nordvpn status";
    sendCommand(vpnProcess, command, 5000);
}
/*
 * RESPONSE IF CONNECTED
 * =====================
 * Status: Connected
 * Current server: nl832.nordvpn.com
 * Country: Netherlands
 * City: Amsterdam
 * Server IP: 178.239.173.183
 * Current technology: OPENVPN
 * Current protocol: UDP
 * Transfer: 23.76 KiB received, 12.28 KiB sent
 * Uptime: 2 minutes 57 seconds
 *
 * RESPONSE IF DISCONNECTED or LOGGED OUT
 * ======================================
 * Status: Disonnected
 *
 */
void QStatusCommand::handleResponse(){
    // Here when response arrives after status request
    if (response().contains("Status: Disconnected")) {
        emit isDisconnected();
        commandSucceeded();
    } else {

        if (response().contains("Status: Connected")) {
            emit isConnected();
        }
        if (response().contains("Current server:")) {
            emit updateServer(parseResponse("Current server:", response()));
        }

        if (response().contains("Country:")) {
            emit updateCountry(parseResponse("Country:", response()));
        }

        if (response().contains("Server IP:")) {
            emit updateIPAddress(parseResponse("Server IP:", response()));
        }

        if (response().contains("Current technology:")) {
            emit updateTechnology(parseResponse("Current technology:", response()));
        }

        if (response().contains("Current protocol:")) {
            emit updateProtocol(parseResponse("Current protocol:", response()));
        }
    }
    QVPNCommand::handleResponse();
}



//NordVPN possible status responses
//Status: Connected --> Means Logged in and connected to VPN
//Status: Disconnected --> Also Means Logged out
//Current server: nl863.nordvpn.com
//Country: Netherlands
//City: Amsterdam
//Server IP: 213.232.87.57
//Current technology: OPENVPN
//Current protocol: UDP


