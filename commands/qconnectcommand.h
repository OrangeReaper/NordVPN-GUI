#ifndef QCONNECTCOMMAND_H
#define QCONNECTCOMMAND_H

#include "commands/qvpncommand.h"

class QConnectCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QConnectCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void execute (QProcess *vpnProcess);
    virtual void handleResponse();
signals:

};

#endif // QCONNECTCOMMAND_H
