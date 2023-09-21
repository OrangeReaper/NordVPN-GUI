#ifndef QGROUPSCOMMAND_H
#define QGROUPSCOMMAND_H

#include <QObject>

#include "commands/qvpncommand.h"

class QGroupsCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QGroupsCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void handleResponse();
signals:
    void groupsUpdated(QStringList countries);
private:
    QStringList m_groups;

};

#endif // QGROUPSCOMMAND_H
