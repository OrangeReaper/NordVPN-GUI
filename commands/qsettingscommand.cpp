#include "qsettingscommand.h"

const int QSettingsCommand::s_whitelists = 2;

QSettingsCommand::QSettingsCommand(QCallback *callback, QObject *parent) : QVPNCommand("nordvpn settings", 5000, callback, parent){
    setLogging(false);
}
/*
 * RESPONSE
 * =========
 * Technology: OPENVPN
 * Protocol: UDP
 * Firewall: enabled
 * Kill Switch: disabled
 * CyberSec: disabled
 * Obfuscate: disabled
 * Notify: disabled
 * Auto-connect: disabled
 * IPv6: disabled
 * DNS: disabled
 * Whitelisted ports:
 *        22 (UDP|TCP)
 * Whitelisted subnets:
 * 	192.168.1.0/24
 */
void QSettingsCommand::handleResponse(){
    // Here when response arrives after settings request
    QSettingsResult result;
    result = parseString("Technology:");
    if (result.hasResult()) emit updateTechnology(result.stringResult());

    result = parseString("Protocol:");
    if (result.hasResult()) emit updateProtocol(result.stringResult());

    result = parseBool("Firewall:");
    if (result.hasResult()) emit updateFirewall(result.boolResult());

    result = parseBool("Kill Switch:");
    if (result.hasResult()) emit updateKillSwitch(result.boolResult());

    result = parseBool("CyberSec:");
    if (result.hasResult()) emit updateCyberSec(result.boolResult());

    result = parseBool("Obfuscate:");
    if (result.hasResult()) emit updateObfuscate(result.boolResult());

    result = parseBool("Notify:");
    if (result.hasResult()) emit updateNotify(result.boolResult());

    result = parseBool("Auto-connect:");
    if (result.hasResult()) emit updateAutoConnect(result.boolResult());

    result = parseBool("IPv6:");
    if (result.hasResult()) emit updateIPv6(result.boolResult());

    result = parseString("DNS:");
    if (result.hasResult()) emit updateDNS(result.stringResult());

    installWhitelist();

    QVPNCommand::handleResponse();
}
QSettingsResult QSettingsCommand::parseString(QString str){
    QSettingsResult result;
    if (response().contains(str)){
        result.setStringResult(parseResponse(str, response()));
        result.setHasResult(true);
    }
    return result;
}
QSettingsResult QSettingsCommand::parseBool(QString str){
    QSettingsResult result;
    if (response().contains(str)){
        QString state = parseResponse(str, response());
        result.setBoolResult(state.contains("enabled", Qt::CaseInsensitive));
        result.setHasResult(true);
    }
    return result;
}
void QSettingsCommand::installWhitelist(){
    // scans through responseList() for whitelisted items and saves them to settings
    QVector<int> result(s_whitelists+1);
    QStringList responseList=response().split("\n");
    result.fill(responseList.size());
    int qtyFound = 0;
    for (int i = 0; i < responseList.size(); ++i){
        const QString &string = responseList.at(i);
        if (string.contains("Allowlisted", Qt::CaseInsensitive))
            result[qtyFound++] = i;
    }

    m_settings.setValue("subnets", "");
    m_settings.setValue("ports", "");
    if (qtyFound > 0){
        for (int i = 0; i < qtyFound; ++i){
            addWhitelistSetting(responseList, result[i], result[i+1]);
        }
    }
}
void QSettingsCommand::addWhitelistSetting(QStringList &list, int from, int to){
    QString targetSetting = "subnets";
    QString targetValue = "";
    if (list.at(from).contains("port", Qt::CaseInsensitive)){
        targetSetting = "ports";
    }
    for (int i = from+1; i < to; ++i){
        if (targetValue.length() > 0)
            targetValue = targetValue.trimmed() + ",";

        targetValue = targetValue + list.at(i).trimmed();
    }

    m_settings.setValue(targetSetting, targetValue);
}
