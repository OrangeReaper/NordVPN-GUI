#ifndef QLOGINCOMMAND_H
#define QLOGINCOMMAND_H

#include "commands/qvpncommand.h"

class QLoginCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QLoginCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void handleResponse();
    virtual void sendCommand();
};

#endif // QLOGINCOMMAND_H


