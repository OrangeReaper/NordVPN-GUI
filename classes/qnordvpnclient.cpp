#include "qnordvpnclient.h"
#include <QMetaEnum>
#include <QCoreApplication>
#include <QAbstractButton>
#include <QMessageBox>

#include "commands/qanycommand.h"
#include "commands/qlogincommand.h"
#include "commands/qconnectcommand.h"
#include "commands/qdisconnectcommand.h"
#include "commands/qstatuscommand.h"
#include "commands/qcountriescommand.h"
#include "commands/qgroupscommand.h"
#include "commands/qlogoutommand.h"
#include "commands/qsettingscommand.h"

#include "classes/staticconstants.h"

QNordVPNClient* QNordVPNClient::s_instance = 0;

QNordVPNClient* QNordVPNClient::getInstance(){
    if(!s_instance) {
        s_instance = new QNordVPNClient();
        s_instance->clearConnectionStatus();
        s_instance->go();
        // There is no instance so we created one.
        return s_instance;
    } else {
        //This is the same instance!
        return s_instance;
    }
}

QNordVPNClient::~QNordVPNClient(){
    QCallback *callback = new QCallback(this);
    setVpnState(Quitting);
    exitVPN(callback);
    if (m_taskQueue != 0) m_taskQueue->deleteLater();
    if (m_vpnProcess != 0) m_vpnProcess->deleteLater();
    if (m_statusTimer !=0) m_statusTimer->deleteLater();
}

void QNordVPNClient::go(){
// Only call during class (singleton) creation
    m_taskQueue = QTaskQueue::getInstance();
    connect(m_taskQueue, SIGNAL(service()), this, SLOT(serviceTasks()));

    QString program = "/bin/sh";
    QStringList arguments;

    m_vpnProcess = new QProcess(this);
    connect(m_vpnProcess , SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
    connect(m_vpnProcess , SIGNAL(started()), this, SLOT(started()));
    connect(m_vpnProcess , SIGNAL(readyReadStandardError()), this, SLOT(error()));
    m_vpnProcess->start(program, arguments);
    emit statusChanged();

    //check status every 5 seconds
    m_statusTimer = new QTimer(this);
    connect(m_statusTimer, SIGNAL(timeout()), this, SLOT(getStatus()));
    m_statusTimer->start(5000);
}
void QNordVPNClient::serviceTasks(){
    if (m_runningTasks < 0) m_runningTasks = 0;
    if (m_runningTasks == 0) runtask();
}
void QNordVPNClient::runtask(){
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    QVPNCommand *vpnCommand = m_taskQueue->dequeue();
    if (m_vpnProcess){
        if (vpnCommand != 0){
            m_runningTasks++;
            connect(vpnCommand->callback(), SIGNAL(callback(QString)), this, SLOT(nextTask()));
            connect(vpnCommand, SIGNAL(failed(QString)), this, SLOT(fatalError(QString)));
            vpnCommand->execute(m_vpnProcess);
        }
    } else {
        m_taskQueue->clear();
    }
}
void QNordVPNClient::nextTask(){
    m_runningTasks--;
    serviceTasks();
}
//*
// CLI Commands
//*
// validate (which nordvpn)
void QNordVPNClient::validateInstallation(QCallback *callback){
    QVPNCommand *vpnCommand = new QAnyCommand(callback, "which nordvpn", 2000, this);
    connect(callback, SIGNAL(callback(QString)), this, SLOT(validate(QString)));
    m_taskQueue->enqueue(vpnCommand);
}
//*
// Supported NordVPN Commands
//*
// account
void QNordVPNClient::accountInfo(QCallback *callback){
    QVPNCommand *vpnCommand = new QAnyCommand(callback, "nordvpn account", 5000, this);
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setAccountExpiry(QString)));
    m_taskQueue->enqueue(vpnCommand);
}
//*
//countries
void QNordVPNClient::refreshCountries(QCallback *callback){
    QVPNCommand *vpnCommand = new QCountriesCommand(callback, this);
    connect(vpnCommand, SIGNAL(countriesUpdated(QStringList)), this, SLOT(setCountries(QStringList)));
    m_taskQueue->enqueue(vpnCommand);
}
//*
//countries
void QNordVPNClient::refreshGroups(QCallback *callback){
    QVPNCommand *vpnCommand = new QGroupsCommand(callback, this);
    connect(vpnCommand, SIGNAL(groupsUpdated(QStringList)), this, SLOT(setGroups(QStringList)));
    m_taskQueue->enqueue(vpnCommand);
}
//*
//login
void QNordVPNClient::loginToAccount(QCallback *callback){
    QVPNCommand *vpnCommand = new QLoginCommand(callback, this);
    connect(vpnCommand, SIGNAL(success()), this, SLOT(loggedIn()));
    m_taskQueue->enqueue(vpnCommand);
    getStatus(new QCallback(this));
}
//*
//logout
void QNordVPNClient::logoutOfAccount(QCallback *callback){
    QVPNCommand *vpnCommand = new QLogoutCommand(callback, this);
    connect(vpnCommand, SIGNAL(success()), this, SLOT(loggedOut()));
    connect(vpnCommand, SIGNAL(failed(QString)), this, SLOT(loggedIn()));
    m_taskQueue->enqueue(vpnCommand);
}
//*
//version
void QNordVPNClient::getVersion(QCallback *callback){
    QVPNCommand *vpnCommand = new QAnyCommand(callback, "nordvpn version", 3000, this);
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setNordVPNVersion(QString)));
    m_taskQueue->enqueue(vpnCommand);
}
//*
//vpn_connect
void QNordVPNClient::connectToVPN(QCallback *callback){
    QVPNCommand *vpnCommand = new QConnectCommand(callback, this);
    connect(vpnCommand, SIGNAL(success()), this, SLOT(connectedOK()));
    // do nothing if connect attempt failed because nothing has changed... Although likely fatal!
    m_taskQueue->enqueue(vpnCommand);
    getStatus(new QCallback(this));
}
//set
void QNordVPNClient::anyCommand(QCallback *callback, QString command, int msTimeout){
    QVPNCommand *vpnCommand = new QAnyCommand(callback, command, msTimeout, this);
    m_taskQueue->enqueue(vpnCommand);
}
//*
//disconnect
void QNordVPNClient::disconnectFromVPN(QCallback *callback){
    QVPNCommand *vpnCommand = new QDisconnectCommand(callback, this);
    connect(vpnCommand, SIGNAL(success()), this, SLOT(loggedIn()));
    // do nothing if disconnect attempt failed because nothing has changed.
    m_taskQueue->enqueue(vpnCommand);
    getStatus(new QCallback(this));
}

//*
//exit
void QNordVPNClient::exitVPN(QCallback *callback){
    QVPNCommand *vpnCommand = new QAnyCommand(callback, "exit", 100, this);
    m_taskQueue->enqueue(vpnCommand);
}
//*
//status
void QNordVPNClient::getStatus(){
    getStatus(new QCallback(this));
}
void QNordVPNClient::removeWhitelistedSubnet(QString subnet){
    QCallback *callback=new QCallback();
    QString command="nordvpn whitelist remove subnet " + subnet;
    anyCommand(callback, command, staticConstants::s_whitelistCommandTimeout);
    getSettings();
}
void QNordVPNClient::removeWhitelistedPort(QString port){
    //'port' could take any of the forms (eg)
    // 9000 - 9100 (UDP|TCP) or
    // 8900 (TCP)
    //

    QString command="nordvpn whitelist remove port";
    if (port.contains("-")){
        command = command + "s ";
    } else {
        command = command + " ";
    }


    int i = port.indexOf("(");
    if (i > 1){
      QString range = port.left(i-1).remove("-");

      QString protocol = " ";
      int j = port.indexOf(")");
      if (!port.contains("UDP|TCP")){
          if (j > 2){
            protocol = protocol + "protocol " + port.mid(i+1,j-i-1);
          }
      }
      if (j > 2){
        QCallback *callback=new QCallback();
        command = command + range + protocol;
        anyCommand(callback, command, staticConstants::s_whitelistCommandTimeout);
        getSettings();
      }
    }

}
void QNordVPNClient::addWhitelistedSubnet(QString subnet){
    QCallback *callback=new QCallback();
    QString command="nordvpn whitelist add subnet " + subnet;
    anyCommand(callback, command, staticConstants::s_whitelistCommandTimeout);
    getSettings();
}
//addWhitelistedPort(ui->lineEdit->text(), ui->lineEdit2->text(), ui->pickTCPUDP->currentIndex());
void QNordVPNClient::addWhitelistedPort(QString port1, QString port2, QString protocol){
    // port1 will always be valid
    // port2 could be empty or be a number bigger than port1
    // if protocol is TCP/UDP then it shouldn't be used (default)
    QString command="nordvpn whitelist add port";
    if (port2.toInt() == 0){
        command = command + " " + port1;
    } else {
        command = command + "s " + port1 + " " + port2;
    }

    if (protocol.length() == 3)
        command = command + " protocol " + protocol;

    QCallback *callback=new QCallback();
    anyCommand(callback, command, staticConstants::s_whitelistCommandTimeout);
    getSettings();

}

void QNordVPNClient::getStatus(QCallback *callback){
    QVPNCommand *vpnCommand = new QStatusCommand(callback, this);
    connect(vpnCommand, SIGNAL(updateCountry(QString)), this, SLOT(setActualCountry(QString)));
    connect(vpnCommand, SIGNAL(updateIPAddress(QString)), this, SLOT(setActualIPAddress(QString)));
    connect(vpnCommand, SIGNAL(updateTechnology(QString)), this, SLOT(setActualTechnology(QString)));
    connect(vpnCommand, SIGNAL(updateProtocol(QString)), this, SLOT(setActualProtocol(QString)));
    connect(vpnCommand, SIGNAL(updateServer(QString)), this, SLOT(setConnectedServer(QString)));
    connect(vpnCommand, SIGNAL(updateUptime(QString)), this, SLOT(setUptime(QString)));
    connect(vpnCommand, SIGNAL(isConnected()), this, SLOT(connectedOK()));
    m_taskQueue->enqueue(vpnCommand);
}
void QNordVPNClient::getSettings(){
    getSettings(new QCallback(this));
}
void QNordVPNClient::getSettings(QCallback *callback){
    QVPNCommand *vpnCommand = new QSettingsCommand(callback, this);
    connect(vpnCommand, SIGNAL(updateTechnology(QString)), this, SLOT(setTechnology(QString)));
    connect(vpnCommand, SIGNAL(updateProtocol(QString)), this, SLOT(setProtocol(QString)));
    connect(vpnCommand, SIGNAL(updateFirewall(bool)), this, SLOT(setFirewall(bool)));
    connect(vpnCommand, SIGNAL(updateKillSwitch(bool)), this, SLOT(setKillSwitch(bool)));
    connect(vpnCommand, SIGNAL(updateCyberSec(bool)), this, SLOT(setCyberSec(bool)));
    connect(vpnCommand, SIGNAL(updateObfuscate(bool)), this, SLOT(setObfuscate(bool)));
    connect(vpnCommand, SIGNAL(updateNotify(bool)), this, SLOT(setNotify(bool)));
    connect(vpnCommand, SIGNAL(updateAutoConnect(bool)), this, SLOT(setAutoConnect(bool)));
    connect(vpnCommand, SIGNAL(updateIPv6(bool)), this, SLOT(setIPv6(bool)));
    connect(vpnCommand, SIGNAL(updateDNS(QString)), this, SLOT(setDNS(QString)));
    connect(vpnCommand->callback(), SIGNAL(callback(QString)), this, SLOT(setWhitelists()));
    m_taskQueue->enqueue(vpnCommand);
}
void QNordVPNClient::error(){
    QString strOut = m_vpnProcess->readAllStandardError();
    emit log(strOut, c_logColor);
    kill();
}
//*
// VPN Process Slots
void QNordVPNClient::started(){
    emit log("VPN Process has Started!", c_logColor);
    validateInstallation(new QCallback(this));
    getVersion(new QCallback(this));
    loginToAccount(new QCallback(this));
    accountInfo(new QCallback(this));
    getStatus(new QCallback(this));
    getSettings(new QCallback(this));
    //refreshCountries(new QCallback(this));
}

void QNordVPNClient::finished(int exitCode, QProcess::ExitStatus exitStatus){
    setVpnState(Dead);
    if (exitStatus == QProcess::NormalExit) {
        emit log("VPN Process finished Normally.", c_logColor);
    } else {
        emit log("VPN Process Crashed.", c_faultColor);
    }
    QString str = "VPN Process Exit Code: " + QString::number(exitCode);
    emit log(str, c_logColor);
    m_vpnProcess->deleteLater();

}
void QNordVPNClient::kill(){
    // If we got gere then something has gone badly wrong
    // So no gracious shutdown here
    if (m_vpnProcess != 0) delete m_vpnProcess;
    m_vpnProcess = 0;
    runtask(); // clear the queue
    emit log("VPN Process has been Killed!", c_faultColor);
}

void QNordVPNClient::setVpnState(vpnState newState){
    if (newState != state){
        state=newState;
        QString status = getVpnStateAsString();
        QString str = "VPN State: " + status;
        emit log (str, c_logColor);
        emit stateChanged(state);
        emit updateStatus(status);
        if (newState == Connected) getStatus(new QCallback(this));
    }
}
QString QNordVPNClient::getVpnStateAsString(){
    QMetaEnum metaEnum = QMetaEnum::fromType<vpnState>();
    QString result = metaEnum.valueToKey(state);
    return result;
}
void QNordVPNClient::sendCommand(QProcess * process, QString command){
    QString str = "Command: " + command;
    emit log(str, c_logColor);
    command = command + " \n";
    QByteArray byteArray = QByteArray::fromStdString(command.toStdString());
    process->write(byteArray);
}

void QNordVPNClient::clearConnectionStatus(){
    setActualCountry("Disconnected");
    setActualIPAddress("Disconnected");
    setActualTechnology("Disconnected");
    setActualProtocol("Disconnected");
    setConnectedServer("Disconnected");
}
void QNordVPNClient::validate(QString response){
    if (response.contains("nordvpn"))
            return;

    fatalError("You need a working installation of the nordvpn app on your system for this application to work.", "FATAL ERROR!");


}
void QNordVPNClient::setNordVPNVersion(QString version){
    int i = version.indexOf("NordVPN Version");
    if (i == -1){
        m_nordVPNVersion = "NordVPN Version: Unknown";
    } else {
        m_nordVPNVersion = version.mid(i).trimmed();
    }
}
void QNordVPNClient::loggedIn(){
    clearConnectionStatus();
    setVpnState(Disconnected);
}
void QNordVPNClient::loggedOut(){
    clearConnectionStatus();
    setVpnState(Dead);
}
void QNordVPNClient::connectedOK(){
    setVpnState(Connected);
}
void QNordVPNClient::setActualCountry(QString country) {
    emit updateActualCountry(country);
}
void QNordVPNClient::setActualIPAddress(QString IPAddress) {
    emit updateActualIPAddress(IPAddress);
}
void QNordVPNClient::setActualTechnology(QString technology) {
    emit updateActualTechnology(technology);
}
void QNordVPNClient::setActualProtocol(QString protocol) {
    emit updateActualProtocol(protocol);
}
void QNordVPNClient::setConnectedServer(QString connectedServer) {
    emit updateConnectedServer(connectedServer);
}
void QNordVPNClient::setUptime(QString uptime) {
    emit updateUptime(uptime);
}
void QNordVPNClient::setCountries(QStringList countries) {
    emit updateCountries(countries);
}
void QNordVPNClient::setGroups(QStringList groups) {
    emit updateGroups(groups);
}
void QNordVPNClient::setTechnology(QString technology){
    emit updateTechnology(technology);
}
void QNordVPNClient::setProtocol(QString protocol){
    emit updateProtocol(protocol);
}
void QNordVPNClient::setFirewall(bool firewall){
    emit updateFirewall(firewall);
}
void QNordVPNClient::setKillSwitch(bool killSwitch){
    emit updateKillSwitch(killSwitch);
}
void QNordVPNClient::setCyberSec(bool cyberSec){
    emit updateCyberSec(cyberSec);
}
void QNordVPNClient::setObfuscate(bool obfuscate){
    emit updateObfuscate(obfuscate);
}
void QNordVPNClient::setNotify(bool notify){
    emit updateNotify(notify);
}
void QNordVPNClient::setAutoConnect(bool autoConnect){
    emit updateAutoConnect(autoConnect);
}
void QNordVPNClient::setIPv6(bool ipV6){
    emit updateIPv6(ipV6);
}
void QNordVPNClient::setDNS(QString dns){
    emit updateDNS(dns);
}
void QNordVPNClient::setWhitelists(){
    emit updateWhitelists();
}
void QNordVPNClient::setAccountExpiry(QString response){
    //Account Information:
    //Email Address: blah@blah.com
    //VPN Service: Active (Expires on May 6th, 2022)
    int j=-1;
    int i = response.indexOf("VPN Service:");
    if (i > 0){
        j = response.indexOf(")", i);
        if (j > 0){
            m_accountStatus = response.mid(i, j-i+1);
        }
    }
    emit accountInfoAvailable(m_accountStatus);
}
void QNordVPNClient::fatalError(QString message, QString text){
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setInformativeText(message);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Retry | QMessageBox::Cancel);

    QAbstractButton *retry = msgBox.button(QMessageBox::Retry);
    retry->setText("Restart");

    QAbstractButton *cancel = msgBox.button(QMessageBox::Cancel);
    cancel->setText("Quit");

    msgBox.setDefaultButton(QMessageBox::Retry);
    msgBox.setIcon(QMessageBox::Critical);
    int ret = msgBox.exec();
    kill();

    QProcess proc;
    QString program = QCoreApplication::applicationFilePath();
    QStringList arguments;
    switch (ret) {
    case QMessageBox::Retry:
        // Restart was clicked
        proc.startDetached(program, arguments);
        QCoreApplication::quit();
        break;
    case QMessageBox::Cancel:
        // Quit was clicked
        QCoreApplication::quit();
        break;
    default:
      break;
    }
}
