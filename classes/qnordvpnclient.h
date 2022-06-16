#ifndef QNORDVPNCLIENT_H
#define QNORDVPNCLIENT_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QStringList>
#include <QSettings>

#include "commands/qvpncommand.h"
#include "classes/qtaskqueue.h"
#include "classes/qcallback.h"

class QNordVPNClient : public QObject
{
    Q_OBJECT
public:
    static QNordVPNClient *getInstance();
    ~QNordVPNClient();

    enum vpnState { Dead, Started, Disconnected, Connected, Quitting };
    Q_ENUM(vpnState);

    enum supportedCommand { account, vpn_connect, countries, disconnect, login, logout, set, settings, status };
    Q_ENUM(supportedCommand);
    //*
    //Commands
    void    connectToVPN(QCallback *callback);
    void    disconnectFromVPN(QCallback *callback);
    void    loginToAccount(QCallback *callback);
    void    logoutOfAccount(QCallback *callback);
    void    getVersion(QCallback *callback);
    void    getStatus(QCallback *callback);
    void    getSettings(QCallback *callback);
    void    anyCommand(QCallback *callback, QString command, int msTimeout=5000);
    void    refreshCountries(QCallback *callback);
    void    refreshGroups(QCallback *callback);
    void    exitVPN(QCallback *callback);
    void    accountInfo(QCallback *callback);
    void    validateInstallation(QCallback *callback);
    void    removeWhitelistedSubnet(QString subnet);
    void    removeWhitelistedPort(QString port);
    void    addWhitelistedSubnet(QString subnet);
    void    addWhitelistedPort(QString port1, QString port2, QString protocol);

    QString getVpnStateAsString();
    vpnState getVpnState() { return state;}

    QString nordVPNVersion()    {return m_nordVPNVersion;}

public slots:
    //*
    //Task Management
    void    serviceTasks();
    void    nextTask();
    //*
    //vpnProcess Management
    void    started();
    void    error();
    void    finished(int exitCode, QProcess::ExitStatus exitStatus);
    void    kill();
    //*
    //Commands
    void    getStatus();    // overloaded
    void    getSettings();  // overloaded;
    //*
    //Command Responses
    //validate
    void    validate(QString response);
    //version
    void    setNordVPNVersion(QString version);
    //account
    void    setAccountExpiry(QString response);
    //*
    //Command Handling
    void    loggedIn();
    void    loggedOut();
    //From NordVPN Status
    void    setActualCountry(QString country);
    void    setActualIPAddress(QString IPAddress);
    void    setActualTechnology(QString technology);
    void    setActualProtocol(QString protocol);
    void    setConnectedServer(QString connectedServer);
    void    connectedOK();  //Status: Connected
    //From NordVPN Settings
    void    setTechnology(QString technology);
    void    setProtocol(QString protocol);
    void    setFirewall(bool firewall);
    void    setKillSwitch(bool killSwitch);
    void    setCyberSec(bool cyberSec);
    void    setObfuscate(bool obfuscate);
    void    setNotify(bool notify);
    void    setAutoConnect(bool autoConnect);
    void    setIPv6(bool ipV6);
    void    setDNS(QString dns);
    void    setWhitelists();

    //From NordVPN Countries
    void    setCountries(QStringList);
    //From NordVPN Groups
    void    setGroups(QStringList);
    //Error Handling
    void    fatalError(QString message, QString text="Error is likely to be fatal!");

signals:
    void    stateChanged(int);
    void    statusChanged();
    void    log(QString, QString);
    //From NordVPN Status
    void    updateActualCountry(QString &country);
    void    updateActualIPAddress(QString &IPAddress);
    void    updateActualTechnology(QString &technology);
    void    updateActualProtocol(QString &protocol);
    void    updateConnectedServer(QString &connectedServer);
    void    updateStatus(QString &status);
    //From NordVPN Settings
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
    void    updateWhitelists();
    //From NordVPN Countries
    void    updateCountries(QStringList &countries);
    //From NordVPN Groups
    void    updateGroups(QStringList &groups);
    //From NoordVPN account
    void    accountInfoAvailable(QString);

private:
    QNordVPNClient(){} // Singleton Object
    static QNordVPNClient *s_instance;

    // Objects
    QProcess    *m_vpnProcess   =0;
    QTaskQueue  *m_taskQueue    =0;
    QTimer      *m_statusTimer  =0;

    void    go();
    void    setVpnState(vpnState newState);
    void    sendCommand(QProcess *process, QString command);

    void    clearConnectionStatus();
    void    runtask();

    int     m_runningTasks=0;

    QString parseResponse(QString find, QString response);

    //From NordVPN Version
    QString m_nordVPNVersion;

    //From NordVPN Account
    QString m_accountStatus = "VPN Service Status: Unknown";

    vpnState state=Dead;

    QSettings m_settings;

    // Constants
    const QString c_logColor   = "#00fdff"; // Cyan
    const QString c_faultColor = "#ff0000"; // Red
;};

#endif // QNORDVPNCLIENT_H
//Current server: nl863.nordvpn.com
//Country: Netherlands
//City: Amsterdam
//Server IP: 213.232.87.57
//Current technology: OPENVPN
//Current protocol: UDP
