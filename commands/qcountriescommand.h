#ifndef QCOUNTRIESCOMMAND_H
#define QCOUNTRIESCOMMAND_H

#include "commands/qvpncommand.h"

class QCountriesCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QCountriesCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void execute (QProcess *vpnProcess);
    virtual void handleResponse();
signals:
    void countriesUpdated(QStringList countries);
private:
    QStringList m_countries;
};

#endif // QCOUNTRIESCOMMAND_H
