#ifndef QLOGINCOMMAND_H
#define QLOGINCOMMAND_H

#include "commands/qvpncommand.h"

class QLoginCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QLoginCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void execute (QProcess *vpnProcess);
    virtual void handleResponse();

};

#endif // QLOGINCOMMAND_H


