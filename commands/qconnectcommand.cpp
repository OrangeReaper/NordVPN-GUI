#include "qconnectcommand.h"
#include <QMetaEnum>
#include <QSettings>
#include "dialogs/controlpanel.h"
#include "classes/staticfunctions.h"

QConnectCommand::QConnectCommand(QCallback *callback, QObject *parent) : QVPNCommand("connect", 20000, callback, parent)
{

}
void QConnectCommand::sendCommand(){
    QSettings settings;

    QString connectionType = settings.value("connectionType").toString();
    QMetaEnum e = QMetaEnum::fromType<ControlPanel::connectionType>();
    QStringList connectionTypes = staticFunctions::enumToStringList(e);
    int i = connectionTypes.indexOf(connectionType);

    QString command = "nordvpn connect ";
    switch (i) {

        case ControlPanel::connectionType::Any:
            break;

        case ControlPanel::connectionType::Country:
            command = command + settings.value("country").toString();
            break;


        case ControlPanel::connectionType::Group:
            command = command + "--group " + settings.value("group").toString();
            break;

        default:
            break;

}
    setCommand(command);
    QVPNCommand::sendCommand();
}
void QConnectCommand::handleResponse(){
    // Here when response arrives after connect request

    if (response().contains("You are connected") || response().contains("Status: Connected")) {
        commandSucceeded();
    }
    QVPNCommand::handleResponse();
}
