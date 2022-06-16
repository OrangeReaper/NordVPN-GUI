#include "controlpanel.h"
#include "ui_controlpanel.h"
#include <mainwindow.h>

#include <QMessageBox>
#include <QFontDatabase>
#include <QMetaEnum>
#include <QScreen>
#include <QDebug>

#include "dialogs/dlgmanage.h"
#include "classes/staticfunctions.h"

ControlPanel::ControlPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    ui->nordvpnGuiVersion->setText("NordVPN-GUI: " + MainWindow::s_version);
    setWindowFlags(Qt::Dialog);

    //*
    //Initialise Status Bar
    m_statusBar = new QStatusBar;
    m_statusBar->layout()->setContentsMargins(0,0,0,0);
    m_statusBar->layout()->setSpacing(0);
    this->layout()->addWidget(m_statusBar);

    m_statusText = new QLabel();
    m_statusText->setMaximumWidth(750);
    QFont font = m_statusText->font();
    font.setPointSize(10);
    font.setItalic(true);
    m_statusText->setFont(font);

    m_statusBar->addWidget(m_statusText, 500);

    //*
    //Initialise connectionType dropdown
    ui->connectionType->blockSignals(true);
    QMetaEnum e = QMetaEnum::fromType<connectionType>();
//    for (int k = 0; k < e.keyCount(); k++)
//    {
//        connectionType type = (connectionType) e.value(k);
//        ui->connectionType->addItem(getConnectionTypeAsString(type));
//    }

    ui->connectionType->addItems(staticFunctions::enumToStringList(e));



    ui->connectionType->setCurrentIndex(ui->connectionType->findText(m_settings.value("connectionType").toString()));
    connectionTypeChanged();
    ui->connectionType->blockSignals(false);

    //*
    //Customise Dialog Buttons
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Exit App");
    QIcon exitIcon(":/r/fontawesome-free-6.0.0-desktop/svgs/solid/arrow-right-to-bracket.svg");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setIcon(exitIcon);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(exitApp()));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked()), this, SLOT(close()));

    //*
    //Initialise Password LineEdit
    m_password = new QPasswordLineEdit(this);
    ui->passwordLayout->insertWidget(1, m_password);
    connect(m_password, SIGNAL(textChanged(QString)), this, SLOT(enableApply()));
    connect(m_password, SIGNAL(textChanged(QString)), this, SLOT(enableCancel()));
    resetAccountSettings(); // Reset values for username and password from settings

    QRect position = frameGeometry();
    position.moveCenter(QGuiApplication::screens().at(0)->availableGeometry().center());
    move(position.topLeft());
    setWindowTitle ("NordVPN-GUI - Control Panel");
    connect(ui->connectionList, SIGNAL(currentTextChanged(QString)), this, SLOT(changeConnection(QString)));

    m_login = new QSliderSwitch(this);
    ui->accountControlLayout->insertWidget(0,m_login);
    m_login->setFixedWidth(130);
    m_login->setOffText("Logged Out");
    m_login->setOnText("Logged in");
    loggedOut(); // default to Logged Out
    connect(m_login, SIGNAL(valueChanged(bool)), this, SLOT(changeLoginState(bool)));

    //*
    //Initialise Widgets in Status Group
    m_connectionControl = new QLabelSwitch(this);
    m_connectionControl->label()->setText("Status:");
    m_connectionControl->sliderSwitch()->setOnText("Connected");
    m_connectionControl->sliderSwitch()->setOffText("Disconnected");
    m_connectionControl->sliderSwitch()->setFixedWidth(150);
    connect(m_connectionControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(controlConnection(bool)));
    ui->statusLabel->deleteLater();
    ui->statusLayout->addWidget(m_connectionControl);

    //initialise subnet and ports lists
    ui->subnetList->setStyleSheet("QListView::item:disabled { color: blue }");
    ui->portList->setStyleSheet("QListView::item:disabled { color: blue }");

    //Initialise Control Widgets
    ui->controls->setLayout(new QVBoxLayout(this));
    ui->controls->layout()->setContentsMargins(0,0,0,0);
    ui->controls->layout()->setSpacing(0);

    m_technologyControl = new QLabelSwitch(this);
    m_technologyControl->sliderSwitch()->setFixedWidth(130);
    m_technologyControl->label()->setText("Technology:");
    m_technologyControl->sliderSwitch()->setOffText("NordLynx");
    m_technologyControl->sliderSwitch()->setOffColor(QColor(c_nordLynxColor));
    m_technologyControl->sliderSwitch()->setOnText("OpenVPN");
    connect(m_technologyControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeTechnology(bool)));
    ui->controls->layout()->addWidget(m_technologyControl);

    m_protocolControl = new QLabelSwitch(this);
    m_protocolControl->sliderSwitch()->setFixedWidth(90);
    m_protocolControl->label()->setText("Protocol:");
    m_protocolControl->sliderSwitch()->setOffText("TCP");
    m_protocolControl->sliderSwitch()->setOffColor(QColor(c_tcpColor));
    m_protocolControl->sliderSwitch()->setOnText("UDP");
    connect(m_protocolControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeProtocol(bool)));
    ui->controls->layout()->addWidget(m_protocolControl);

    m_firewallControl = new QLabelSwitch(this);
    m_firewallControl->label()->setText("Firewall:");
    connect(m_firewallControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeFirewall(bool)));
    ui->controls->layout()->addWidget(m_firewallControl);

    m_killSwitchControl = new QLabelSwitch(this);
    m_killSwitchControl->label()->setText("Kill Switch:");
    m_killSwitchControl->setToolTip("Firewall must be enabled to use Kill Switch");
    connect(m_killSwitchControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeKillSwitch(bool)));
    ui->controls->layout()->addWidget(m_killSwitchControl);

    m_cyberSecControl = new QLabelSwitch(this);
    m_cyberSecControl->label()->setText("CyberSec:");
    m_cyberSecControl->setToolTip("DNS must be disabled to use CyberSec feature");
    connect(m_cyberSecControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeCyberSec(bool)));
    ui->controls->layout()->addWidget(m_cyberSecControl);

    m_obfuscateControl = new QLabelSwitch(this);
    m_obfuscateControl->label()->setText("Obfuscate:");
    connect(m_obfuscateControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeObfuscate(bool)));
    ui->controls->layout()->addWidget(m_obfuscateControl);

    m_notifyControl = new QLabelSwitch(this);
    m_notifyControl->label()->setText("Notify:");
    connect(m_notifyControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeNotify(bool)));
    ui->controls->layout()->addWidget(m_notifyControl);

    m_autoConnectControl = new QLabelSwitch(this);
    m_autoConnectControl->label()->setText("Auto-connect:");
    connect(m_autoConnectControl->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeAutoConnect(bool)));
    ui->controls->layout()->addWidget(m_autoConnectControl);

    m_ipV6Control = new QLabelSwitch(this);
    m_ipV6Control->label()->setText("IPv6:");
    connect(m_ipV6Control->sliderSwitch(), SIGNAL(valueChanged(bool)), this, SLOT(changeIPv6(bool)));
    ui->controls->layout()->addWidget(m_ipV6Control);

    ui->dns->setText(m_settings.value("dns").toString());
    m_dnsControl = new QSliderSwitch(this);
    m_dnsControl->setFixedWidth(110);
    connect(m_dnsControl, SIGNAL(valueChanged(bool)), this, SLOT(changeDNS(bool)));
    ui->dnsTitleLayout->insertWidget(1,m_dnsControl);
    ui->dnsTitleLayout->insertStretch(2,100);


    m_overlay = new QOverlay(this);
    m_overlay->setFixedWidth(this->width());
    m_overlay->setMaxHeight(c_overlayRatio * this->height() / 100); 
    connect(m_overlay, SIGNAL(reposition()), this, SLOT(repositionOverlay()));
    connect(m_overlay, SIGNAL(logged(QString)), m_statusText, SLOT(setText(QString)));
    m_overlay->show();

    this->setFixedSize(this->width(),this->height());
//    qDebug() << "Width @ instantiation: " << this->width();
//    qDebug() << "Height @ instantiation: " << this->height();



}

ControlPanel::~ControlPanel()
{
    delete ui;
}
void ControlPanel::resizeEvent(QResizeEvent* resizeEvent){
    QSettings settings;
    settings.setValue("controlPanelGeometry", saveGeometry());
//    qDebug() << "Width b4 reposition: " << this->width();
    repositionOverlay();
//    qDebug() << "Width after reposition: " << this->width();
    QDialog::resizeEvent(resizeEvent);
    m_overlay->contract();

}
void ControlPanel::showEvent(QShowEvent *showEvent){
    ui->nordvpnVersion->setText(m_nordVPNClient->nordVPNVersion());
    QDialog::showEvent(showEvent);
    if (runOnce){
        m_overlay->contract();
        runOnce=false;
    }
}
void ControlPanel::resetAccountSettings(){
    // Reset values for username and password from settings
    // and disable buttons until the user changes something
    ui->username->blockSignals(true);
    ui->username->setText(m_settings.value("username").toString());
    ui->username->blockSignals(false);

    m_password->blockSignals(true);
    m_password->setText(m_settings.value("password").toString());
    m_password->blockSignals(false);

    ui->applyButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);
}
void ControlPanel::applyNewAccountSettings(){
    m_settings.setValue("username", ui->username->text());
    m_settings.setValue("password", m_password->text());
    if (m_nordVPNClient->getVpnState() == QNordVPNClient::vpnState::Dead) {
        cmdLogin();
    }
    ui->applyButton->setEnabled(false);
}

void ControlPanel::cmdLogin(){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback()), this, SLOT(setLoginState()));
    m_nordVPNClient->loginToAccount(callback);
}
void ControlPanel::cmdLogout(){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback()), this, SLOT(setLoginState()));
    m_nordVPNClient->logoutOfAccount(callback);
}
void ControlPanel::cmdConnect(){
    m_connectionControl->sliderSwitch()->setOnText("Connecting");
    m_connectionControl->sliderSwitch()->setValue(true);
    QCallback *callback=new QCallback();
    m_nordVPNClient->connectToVPN(callback);
}
void ControlPanel::cmdDisconnect(){
    m_connectionControl->sliderSwitch()->setOffText("Disconnecting");
    m_connectionControl->sliderSwitch()->setValue(false);
    QCallback *callback=new QCallback();
    m_nordVPNClient->disconnectFromVPN(callback);
}
void ControlPanel::controlConnection(bool connect){
    if (connect){
        cmdConnect();
    } else {
        cmdDisconnect();
    }
}
void ControlPanel::enableApply(){
    ui->applyButton->setEnabled(true);
}
void ControlPanel::enableCancel(){
    ui->cancelButton->setEnabled(true);
}
void ControlPanel::setActualStatus(QString &actualStatus){
    if (actualStatus == "Connected"){
        m_connectionControl->sliderSwitch()->setValue(true);
        m_connectionControl->sliderSwitch()->setOnText(actualStatus);
    } else {
       m_connectionControl->sliderSwitch()->setValue(false);
       m_connectionControl->sliderSwitch()->setOffText(actualStatus);
    }
}
void ControlPanel::setActualServer(QString &actualServer){
    ui->actualServer->setText(actualServer);
}
void ControlPanel::setActualCountry(QString &actualCountry){
    ui->actualCountry->setText(actualCountry);
}
void ControlPanel::setActualIP(QString &actualIP){
    ui->actualIP->setText(actualIP);
}
void ControlPanel::setActualTechnology(QString &actualTechnology){
    ui->actualTechnology->setText(actualTechnology);
}
void ControlPanel::setActualProtocol(QString &actualProtocol){
    ui->actualProtocol->setText(actualProtocol);
}
void ControlPanel::setCountries(QStringList &countries){
    populateConnectionList(countries, m_settings.value("country").toString());
//    ui->connectionList->blockSignals(true);
//    if (countries.length()>0){
//        ui->connectionList->clear();
//        ui->connectionList->addItems(countries);
//    }
//    int idx = ui->connectionList->findText();
//    if (idx != -1) ui->connectionList->setCurrentIndex(idx);
//    ui->connectionList->setVisible(true);
//    ui->labelConnectionList->setVisible(true);

//    ui->connectionList->blockSignals(false);
}
void ControlPanel::setGroups(QStringList &groups){
    populateConnectionList(groups, m_settings.value("group").toString());
}
void ControlPanel::populateConnectionList(QStringList &list, QString setValue){
    ui->connectionList->blockSignals(true);
    if (list.length()>0){
        ui->connectionList->clear();
        ui->connectionList->addItems(list);
    }
    int idx = ui->connectionList->findText(setValue);
    if (idx != -1) ui->connectionList->setCurrentIndex(idx);
    ui->connectionList->setVisible(true);
    ui->labelConnectionList->setVisible(true);

    ui->connectionList->blockSignals(false);
}
void ControlPanel::setTechnology(QString technology){
    m_technologyControl->sliderSwitch()->setValue(technology.contains("openvpn", Qt::CaseInsensitive));
}
void ControlPanel::setProtocol(QString protocol){
    m_protocolControl->sliderSwitch()->setValue(protocol.contains("udp", Qt::CaseInsensitive));
}
void ControlPanel::setFirewall(bool firewall){
    m_firewallControl->sliderSwitch()->setValue(firewall);
    m_killSwitchControl->sliderSwitch()->setEnabled(firewall);
}
void ControlPanel::setKillSwitch(bool killSwitch){
    m_killSwitchControl->sliderSwitch()->setValue(killSwitch);
}
void ControlPanel::setCyberSec(bool cyberSec){
    m_cyberSecControl->sliderSwitch()->setValue(cyberSec);
}
void ControlPanel::setObfuscate(bool obfuscate){
    m_obfuscateControl->sliderSwitch()->setValue(obfuscate);
}
void ControlPanel::setNotify(bool notify){
    m_notifyControl->sliderSwitch()->setValue(notify);
}
void ControlPanel::setAutoConnect(bool autoConnect){
    m_autoConnectControl->sliderSwitch()->setValue(autoConnect);
}
void ControlPanel::setIPv6(bool ipV6){
    m_ipV6Control->sliderSwitch()->setValue(ipV6);
}
void ControlPanel::setDNS(QString dns){
    m_dnsControl->blockSignals(true);

    if (dns.contains("disabled", Qt::CaseInsensitive)){
        m_dnsControl->setValue(false);
        // Disabling DNS allows cyberSec
        m_cyberSecControl->sliderSwitch()->setEnabled(true);
    } else {
        m_dnsControl->setValue(true);
        // Enabling DNS disables cyberSec
        m_cyberSecControl->sliderSwitch()->blockSignals(true);
        m_cyberSecControl->sliderSwitch()->setValue(false);
        m_cyberSecControl->sliderSwitch()->setEnabled(false);
        m_cyberSecControl->sliderSwitch()->blockSignals(false);

        ui->dns->setText(m_settings.value("dns").toString());
    }

    m_dnsControl->blockSignals(false);
}
void ControlPanel::setWhitelists(){
    populateListFromSetting("subnets", ui->subnetList);
    populateListFromSetting("ports", ui->portList);
}
void ControlPanel::populateListFromSetting(QString setting, QListWidget *list){
    list->clear();
    QSettings settings;
    QStringList items = settings.value(setting).toString().split(",");
    foreach (const QString &item, items) {
        QListWidgetItem *nextItem = new QListWidgetItem(item, list);
        nextItem->setFlags(Qt::ItemFlags());
    }
}
//controls
void ControlPanel::changeTechnology(bool openvpn){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setTechnologyState(QString)));
    QString command="nordvpn set technology NordLynx";
    if (openvpn){
        command="nordvpn set technology OpenVPN";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeProtocol(bool udp){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setProtocolState(QString)));
    QString command="nordvpn set protocol TCP";
    if (udp){
        command="nordvpn set protocol UDP";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeFirewall(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setFirewallState(QString)));
    QString command="nordvpn set firewall disabled";
    if (enabled){
        command="nordvpn set firewall enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
    m_nordVPNClient->getSettings();
}
void ControlPanel::changeKillSwitch(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setKillSwitchState(QString)));
    QString command="nordvpn set killswitch disabled";
    if (enabled){
        command="nordvpn set killswitch enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeCyberSec(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setCyberSecState(QString)));
    QString command="nordvpn set cybersec disabled";
    if (enabled){
        command="nordvpn set cybersec enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeObfuscate(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setObfuscateState(QString)));
    QString command="nordvpn set obfuscate disabled";
    if (enabled){
        command="nordvpn set obfuscate enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeNotify(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setNotifyState(QString)));
    QString command="nordvpn set notify disabled";
    if (enabled){
        command="nordvpn set notify enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeAutoConnect(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setAutoConnectState(QString)));
    QString command="nordvpn set autoconnect disabled";
    if (enabled){
        command="nordvpn set autoconnect enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeIPv6(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setIPv6State(QString)));
    QString command="nordvpn set ipv6 disabled";
    if (enabled){
        command="nordvpn set ipv6 enabled";
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::changeDNS(bool enabled){
    QCallback *callback=new QCallback();
    connect(callback, SIGNAL(callback(QString)), this, SLOT(setDNS(QString)));
    QString command="nordvpn set dns disabled";
    if (enabled) {
        command="nordvpn set dns " + m_settings.value("dns").toString().replace(","," ");
    }
    m_nordVPNClient->anyCommand(callback, command, m_controlTimeout);
}
void ControlPanel::manageDNS(){
    dlgManage manageDNS(this);
    manageDNS.setWindowTitle("Manage DNS Addresses");
    if (m_dnsControl->value())
        connect(&manageDNS, SIGNAL(applyDNS(bool)), this, SLOT(changeDNS(bool)));
    manageDNS.exec();
}
void ControlPanel::manageSubnets(){
    dlgManage manageSubnet(this, dlgManage::manageMode::subnet, 255);
    manageSubnet.setWindowTitle("Manage Allowed Subnets");
    manageSubnet.exec();

}
void ControlPanel::managePorts(){
    dlgManage manageSubnet(this, dlgManage::manageMode::port, 255);
    manageSubnet.setWindowTitle("Manage Allowed Ports");
    manageSubnet.exec();

}
//*
//Control callbacks
void ControlPanel::setTechnologyState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_technologyControl);
}
void ControlPanel::setProtocolState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_protocolControl);
}
void ControlPanel::setFirewallState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive)){
        return;
    }

    toggleSwitch(m_firewallControl);
}
void ControlPanel::setKillSwitchState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_killSwitchControl);
}
void ControlPanel::setCyberSecState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_cyberSecControl);
}
void ControlPanel::setObfuscateState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_obfuscateControl);
}
void ControlPanel::setNotifyState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_notifyControl);
}
void ControlPanel::setAutoConnectState(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_autoConnectControl);
}
void ControlPanel::setIPv6State(QString response){
    if (response.contains(" successfully", Qt::CaseInsensitive) || response.contains(" already", Qt::CaseInsensitive))
        return;

    toggleSwitch(m_ipV6Control);
}
void ControlPanel::toggleSwitch(QLabelSwitch *labelSwitch){
    labelSwitch->sliderSwitch()->setValue(!labelSwitch->sliderSwitch()->value());
}
void ControlPanel::changeConnection(QString newConnection){

    switch (m_currentConnectionType) {

        case connectionType::Any:
            break;

        case connectionType::Country:
            if (newConnection != m_settings.value("country").toString()){
                if (disconnected()){
                    m_settings.setValue("country", newConnection);
                    if (m_nordVPNClient->getVpnState() == QNordVPNClient::vpnState::Connected) m_nordVPNClient->connectToVPN(new QCallback(this));
                } else {
                    QStringList nullList;
                    setCountries(nullList);
                }
            }
            break;


        case connectionType::Group:
            if (newConnection != m_settings.value("group").toString()){
                if (disconnected()){
                    m_settings.setValue("group", newConnection);
                    if (m_nordVPNClient->getVpnState() == QNordVPNClient::vpnState::Connected) m_nordVPNClient->connectToVPN(new QCallback(this));
                } else {
                    QStringList nullList;
                    setCountries(nullList);
                }
            }
            break;

        default:
            break;

    }

}
void ControlPanel::exitApp(){
    QCoreApplication::quit();
}
void ControlPanel::changeLoginState(bool doLogin){
    if (doLogin){
        cmdLogin();
    } else {
        cmdLogout();
    }
}

bool ControlPanel::disconnected(){

    if (m_nordVPNClient->getVpnState() != QNordVPNClient::vpnState::Connected){
        return true;
    } else {
        QMessageBox msgBox;
        msgBox.setText("WARNING: Changing this setting may temporarily disconnect the VPN");
        msgBox.setInformativeText("Do you want to save your change?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Yes:
            // Yes was clicked
            return true;
            break;
          case QMessageBox::No:
            // No was clicked
            return false;
            break;
          default:
              // should never be reached
              break;
        }
    }
    return false; // should never be reached
}
void ControlPanel::loggedIn(){
    m_login->blockSignals(true);
    m_login->setValue(true);
    m_login->setToolTip("Click to Logout");
    m_login->setEnabled(true);
    m_login->blockSignals(false);
}
void ControlPanel::loggedOut(){
    m_login->blockSignals(true);
    m_login->setValue(false);
    m_login->setEnabled(true);
    m_login->setToolTip("Click to Login");
    m_login->blockSignals(false);
}
void ControlPanel::VPNChangedState(){
    setLoginState();
}
void ControlPanel::setLoginState(){
    if (m_nordVPNClient->getVpnState() == QNordVPNClient::vpnState::Dead){
        loggedOut();
    } else {
        loggedIn();
    }
}
void ControlPanel::setAccountInfo(QString info){
    ui->accoutInfo->setText(info);
}
void ControlPanel::repositionOverlay(){
    m_overlay->setFixedWidth(this->width());
}
QString ControlPanel::getConnectionTypeAsString(connectionType type){
    QMetaEnum metaEnum = QMetaEnum::fromType<connectionType>();
    QString result = metaEnum.valueToKey(type);
    return result;
}
void ControlPanel::connectionTypeChanged(){

    ui->connectionList->setVisible(false);
    ui->labelConnectionList->setVisible(false);

    int i = ui->connectionType->currentIndex();
    m_settings.setValue("connectionType", ui->connectionType->currentText());

        switch (i) {

            case connectionType::Any:
                m_currentConnectionType = Any;
                break;

            case connectionType::Country:
                m_currentConnectionType = Country;
                m_nordVPNClient->refreshCountries(new QCallback(this));
                break;


            case connectionType::Group:
                m_currentConnectionType = Group;
                m_nordVPNClient->refreshGroups(new QCallback(this));
                break;

            default:
                break;

    }

}
