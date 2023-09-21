#ifndef QDISCONNECTCOMMAND_H
#define QDISCONNECTCOMMAND_H

#include "commands/qvpncommand.h"

class QDisconnectCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QDisconnectCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void handleResponse();
signals:

};

#endif // QDISCONNECTCOMMAND_H
