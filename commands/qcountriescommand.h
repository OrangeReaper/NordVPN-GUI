#ifndef QCOUNTRIESCOMMAND_H
#define QCOUNTRIESCOMMAND_H

#include "commands/qvpncommand.h"

class QCountriesCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QCountriesCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void handleResponse();

    QStringList responseList() { return m_responseList; }
signals:
    void countriesUpdated(QStringList countries);
private:
    QStringList m_countries;

    QStringList m_responseList;
};

#endif // QCOUNTRIESCOMMAND_H
