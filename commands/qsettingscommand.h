#ifndef QSETTINGSCOMMAND_H
#define QSETTINGSCOMMAND_H

#include <QSettings>

#include "commands/qvpncommand.h"
#include "classes/qsettingsresult.h"

class QSettingsCommand : public QVPNCommand
{
    Q_OBJECT
public:
    explicit QSettingsCommand(QCallback *callback, QObject *parent = nullptr);
    virtual void execute (QProcess *vpnProcess);
    virtual void handleResponse();

signals:
    void    updateTechnology(QString technology);
    void    updateProtocol(QString protocol);
    void    updateFirewall(bool firewall);
    void    updateKillSwitch(bool killSwitch);
    void    updateCyberSec(bool cyberSec);
    void    updateObfuscate(bool obfuscate);
    void    updateNotify(bool notify);
    void    updateAutoConnect(bool autoConnect);
    void    updateIPv6(bool ipV6);
    void    updateDNS(QString dns);
private:
    void installWhitelist();
    void addWhitelistSetting(int from, int to);

    static const int s_whitelists;

    QSettingsResult parseString(QString str);
    QSettingsResult parseBool(QString str);

    QSettings m_settings;
};


#endif // QSETTINGSCOMMAND_H
