#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "classes/clsettings.h"

#include <QSettings>
#include <QDebug>
#include <QHBoxLayout>

const QString MainWindow::s_version = QString("v0.114.1");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setToolTip(s_version); // <==================================================================================================  VERSION NUMBER

    QCoreApplication::setOrganizationName("abondServices");//(Strings::organisationName);
    QCoreApplication::setOrganizationDomain("abondservices.co.uk");//(Strings::organisationDomain);
    QCoreApplication::setApplicationName("NordVPN-GUI");//(Strings::applicationName);

    clSettings::validate();                         //Initialise settings
    QSettings settings;


    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    this->statusBar()->setSizeGripEnabled(false);

    int iconsize=settings.value("iconSize").toInt();
    pgmImage = new QCustomImage(this, ":/r/images/insecure.png",iconsize,iconsize);
    resize(iconsize,iconsize);




    //*
    //Initialise Main Window display
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(pgmImage);
    setCentralWidget(new QWidget);
    this->centralWidget()->setStyleSheet("background-color: rgba(255, 0, 0, 0)"); //Transparency
    centralWidget()->setLayout(layout);

    //*
    //Initialise Singletons
    m_nordVPNClient = QNordVPNClient::getInstance();
    m_logger = QLogger::getInstance();
    connect(m_nordVPNClient, SIGNAL(log(QString, QString)), m_logger, SLOT(logSomething(QString, QString)));
    connect(m_nordVPNClient, SIGNAL(stateChanged(int)), this, SLOT(VPNChangedState(int)));

    //*
    //Initialise Control Panel
    m_controlPanel = new ControlPanel(this);
    connect(m_logger, SIGNAL(log(QString, QString)), m_controlPanel->overlay(), SLOT(logSomething(QString, QString)));
    connect(m_nordVPNClient, SIGNAL(updateActualCountry(QString&)), m_controlPanel, SLOT(setActualCountry(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateActualIPAddress(QString&)), m_controlPanel, SLOT(setActualIP(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateActualTechnology(QString&)), m_controlPanel, SLOT(setActualTechnology(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateActualProtocol(QString&)), m_controlPanel, SLOT(setActualProtocol(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateConnectedServer(QString&)), m_controlPanel, SLOT(setActualServer(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateUptime(QString&)), m_controlPanel, SLOT(setUptime(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateStatus(QString&)), m_controlPanel, SLOT(setActualStatus(QString&)));
    connect(m_nordVPNClient, SIGNAL(updateCountries(QStringList&)), m_controlPanel, SLOT(setCountries(QStringList&)));
    connect(m_nordVPNClient, SIGNAL(updateGroups(QStringList&)), m_controlPanel, SLOT(setGroups(QStringList&)));
    connect(m_nordVPNClient, SIGNAL(stateChanged(int)), m_controlPanel, SLOT(VPNChangedState()));
    connect(m_nordVPNClient, SIGNAL(accountInfoAvailable(QString)), m_controlPanel, SLOT(setAccountInfo(QString)));

    connect(m_nordVPNClient, SIGNAL(updateTechnology(QString)), m_controlPanel, SLOT(setTechnology(QString)));
    connect(m_nordVPNClient, SIGNAL(updateProtocol(QString)), m_controlPanel, SLOT(setProtocol(QString)));
    connect(m_nordVPNClient, SIGNAL(updateFirewall(bool)), m_controlPanel, SLOT(setFirewall(bool)));
    connect(m_nordVPNClient, SIGNAL(updateKillSwitch(bool)), m_controlPanel, SLOT(setKillSwitch(bool)));
    connect(m_nordVPNClient, SIGNAL(updateCyberSec(bool)), m_controlPanel, SLOT(setCyberSec(bool)));
    connect(m_nordVPNClient, SIGNAL(updateObfuscate(bool)), m_controlPanel, SLOT(setObfuscate(bool)));
    connect(m_nordVPNClient, SIGNAL(updateNotify(bool)), m_controlPanel, SLOT(setNotify(bool)));
    connect(m_nordVPNClient, SIGNAL(updateAutoConnect(bool)), m_controlPanel, SLOT(setAutoConnect(bool)));
    connect(m_nordVPNClient, SIGNAL(updateIPv6(bool)), m_controlPanel, SLOT(setIPv6(bool)));
    connect(m_nordVPNClient, SIGNAL(updateDNS(QString)), m_controlPanel, SLOT(setDNS(QString)));
    connect(m_nordVPNClient, SIGNAL(updateWhitelists()), m_controlPanel, SLOT(setWhitelists()));

    m_controlPanel->connectionTypeChanged();

    //*
    //Add Actions to Main Window
    a_controlPanel = new QAction("Control Panel", this);
    connect(a_controlPanel, SIGNAL(triggered()), this, SLOT(controlPanel()));
    pgmImage->menu()->addAction(a_controlPanel);

    a_enable = new QAction("Connect", this);
    connect(a_enable, SIGNAL(triggered()), m_controlPanel, SLOT(cmdConnect()));
    pgmImage->menu()->addAction(a_enable);

    a_disable = new QAction("Disconnect", this);
    connect(a_disable, SIGNAL(triggered()), m_controlPanel, SLOT(cmdDisconnect()));
    pgmImage->menu()->addAction(a_disable);

    a_disQuit = new QAction("Disconnect && Quit", this);
    connect(a_disQuit, SIGNAL(triggered()), m_controlPanel, SLOT(cmdDisQuit()));
    pgmImage->menu()->addAction(a_disQuit);

    a_quit = new QAction("Quit", this);
    connect(a_quit, SIGNAL(triggered()), this, SLOT(userClose()));
    pgmImage->menu()->addAction(a_quit);

    //*
    // Assume not logged in to begin with.
    a_enable->setVisible(true);
    a_disable->setVisible(false);

    if (settings.allKeys().contains("controlPanelGeometry")){
        //m_controlPanel->restoreGeometry(settings.value("controlPanelGeometry").toByteArray());
    }


}

MainWindow::~MainWindow()
{
    saveWindowState();
    delete ui;
    if (m_nordVPNClient != 0) m_nordVPNClient->deleteLater();
    if (m_logger != 0) m_logger->deleteLater();
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    m_pos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - m_pos;
        QPoint newpos = this->pos() + diff;

        this->move(newpos);
    }
}

void MainWindow::controlPanel(){
    m_controlPanel->show();
}
void MainWindow::userClose(){
    QCoreApplication::quit();
}
void MainWindow::saveWindowState(){
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}
void MainWindow::VPNChangedState(int state){
    if (state == QNordVPNClient::Connected){
        pgmImage->loadImage(":/r/images/secure.png");
        a_enable->setVisible(false);
        a_disQuit->setVisible(true);
    } else {
        pgmImage->loadImage(":/r/images/insecure.png");
        a_enable->setVisible(true);
        a_disQuit->setVisible(false);
    }
    a_disable->setVisible(!a_enable->isVisible());
}
