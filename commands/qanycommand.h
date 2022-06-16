#ifndef QANYCOMMAND_H
#define QANYCOMMAND_H

#include "commands/qvpncommand.h"

class QAnyCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QAnyCommand(QCallback *callback, QString command, int msTimeout, QObject *parent = nullptr);
    virtual void execute (QProcess *vpnProcess);
signals:

private:
    QString m_command;
    int     m_timeout;
};

#endif // QANYCOMMAND_H
