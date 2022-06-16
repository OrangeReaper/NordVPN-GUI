#ifndef QLOGOUTOMMAND_H
#define QLOGOUTOMMAND_H

#include "commands/qvpncommand.h"

class QLogoutCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QLogoutCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void execute (QProcess *vpnProcess);
    virtual void handleResponse();

signals:

};

#endif // QLOGOUTOMMAND_H
