#ifndef QSTATUSCOMMAND_H
#define QSTATUSCOMMAND_H

#include "commands/qvpncommand.h"

class QStatusCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QStatusCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void handleResponse();
signals:
    void    updateCountry(QString country);
    void    updateIPAddress(QString IPAddress);
    void    updateTechnology(QString technology);
    void    updateProtocol(QString protocol);
    void    updateServer(QString connectedServer);
    void    updateUptime(QString uptime);
    void    isConnected();     //Status: Connected
    void    isDisconnected();  //Status: Disconnected
private:

//    int m_updatedMetrics;
};

#endif // QSTATUSCOMMAND_H
