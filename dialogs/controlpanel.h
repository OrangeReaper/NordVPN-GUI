#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDialog>
#include <QSettings>
#include <QStatusBar>
#include <QListWidget>

#include "classes/qnordvpnclient.h"
#include "classes/qoverlay.h"
#include "widgets/QPasswordLineEdit/qpasswordlineedit.h"
#include "widgets/QLabelSwitch/qlabelswitch.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QDialog
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    enum connectionType { Any=0, Group=1, Country=2 };
    Q_ENUM(connectionType);

    void resizeEvent(QResizeEvent *resizeEvent);
    void showEvent(QShowEvent *showEvent);

    QString getConnectionTypeAsString(connectionType type);

    //properties
    QOverlay *overlay() { return m_overlay; }

    void setControlTimeout(int msTimeout) { m_controlTimeout = msTimeout; }
    int  controlTimeout()                 { return m_controlTimeout; }

public slots:
    void applyNewAccountSettings();
    void enableApply();
    void resetAccountSettings();
    void enableCancel();
    //*
    //From NordVPN Status
    void setActualStatus(QString &actualStatus);
    void setActualServer(QString &actualServer);
    void setActualCountry(QString &actualCountry);
    void setActualIP(QString &actualIP);
    void setActualTechnology(QString &actualTechnology);
    void setActualProtocol(QString &actualProtocol);
    //*
    //NordVPN Information
    void setCountries(QStringList &countries);
    void setGroups(QStringList &groups);
    //*
    //From NordVPN Settings
    void setTechnology(QString technology);
    void setProtocol(QString protocol);
    void setFirewall(bool firewall);
    void setKillSwitch(bool killSwitch);
    void setCyberSec(bool cyberSec);
    void setObfuscate(bool obfuscate);
    void setNotify(bool notify);
    void setAutoConnect(bool autoConnect);
    void setIPv6(bool ipV6);
    void setDNS(QString dns);
    void setWhitelists();
    //*
    //Controls
    void changeTechnology(bool openvpn);
    void changeProtocol(bool udp);
    void changeFirewall(bool enabled);
    void changeKillSwitch(bool enabled);
    void changeCyberSec(bool enabled);
    void changeObfuscate(bool enabled);
    void changeNotify(bool enabled);
    void changeAutoConnect(bool enabled);
    void changeIPv6(bool enabled);
    void changeConnection(QString newConnection);
    void changeLoginState(bool login);
    void manageDNS();
    void manageSubnets();
    void managePorts();
    void changeDNS(bool enabled);
    //*
    //Control Callbacks
    void setTechnologyState(QString response);
    void setProtocolState(QString response);
    void setFirewallState(QString response);
    void setKillSwitchState(QString response);
    void setCyberSecState(QString response);
    void setObfuscateState(QString response);
    void setNotifyState(QString response);
    void setAutoConnectState(QString response);
    void setIPv6State(QString response);

    void connectionTypeChanged();
    void setLoginState();
    void VPNChangedState();
    void controlConnection(bool connect);
    void setAccountInfo(QString info);
    void repositionOverlay();
    void exitApp(); // used to exit application
    //*
    //Login/Logout commands
    void cmdConnect();
    void cmdDisconnect();
private:
    Ui::ControlPanel *ui;

    void loggedIn();
    void loggedOut();
    void cmdLogin();
    void cmdLogout();

    void toggleSwitch(QLabelSwitch *labelSwitch);
    void populateListFromSetting(QString setting, QListWidget *list);
    void populateConnectionList(QStringList &list, QString setValue);

    bool disconnected();
    bool runOnce=true;

    QSettings m_settings;

    connectionType  m_currentConnectionType;

    QNordVPNClient  *m_nordVPNClient=QNordVPNClient::getInstance();

    //properties
    int m_controlTimeout=5000;

    //Controls
    QSliderSwitch   *m_login;
    QLabelSwitch    *m_connectionControl;
    QLabelSwitch    *m_technologyControl;
    QLabelSwitch    *m_protocolControl;
    QLabelSwitch    *m_firewallControl;
    QLabelSwitch    *m_killSwitchControl;
    QLabelSwitch    *m_cyberSecControl;
    QLabelSwitch    *m_obfuscateControl;
    QLabelSwitch    *m_notifyControl;
    QLabelSwitch    *m_autoConnectControl;
    QLabelSwitch    *m_ipV6Control;
    QSliderSwitch   *m_dnsControl;

    QPasswordLineEdit *m_password;

    //Widgets
    QStatusBar      *m_statusBar;
    QLabel          *m_statusText;

    //Overlay
    QOverlay        *m_overlay;

    //Constants
    int c_overlayRatio=75; //percentage of control panel that overlay will use when active
    const QString c_tcpColor="#ffcc99"; // Orange
    const QString c_nordLynxColor="#99ccff"; // Blue

};

#endif // CONTROLPANEL_H
