#include "qvpncommand.h"
#include <QDebug>
#include <QRegularExpression>

#include "classes/staticfunctions.h"

QVPNCommand::QVPNCommand(QCallback *callback, QObject *parent) : QObject(parent){
    m_callback=callback;
}
QVPNCommand::~QVPNCommand(){
    //qDebug() << "QVPNCommand Destroyed";
}
void QVPNCommand::handleResponse(){
//    if (m_response.contains(c_commandComplete)) {
//        finished();
//    }
    // MUST BE SUBCLASSED
}
void QVPNCommand::execute (QProcess *vpnProcess){
}
void QVPNCommand::handleTimeout(){
    commandFailed();
}
void QVPNCommand::sendCommand(QProcess *vpnProcess, QString command, int timeout){
    connect(this, SIGNAL(log(QString,QString)), QLogger::getInstance(), SLOT(logSomething(QString,QString)));
    m_timer=new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timedOut()));

    m_vpnProcess=vpnProcess;
    connect(vpnProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getResponse()));

    // Prepare command
    QString str = "C: " + command;
    logSomething(str, c_commandColor);
    m_lastCommand = str;
    command = command + "; echo " + c_commandComplete + " \n";
    QByteArray byteArray = QByteArray::fromStdString(command.toStdString());

    // Send command and start timeout
    vpnProcess->write(byteArray);
    m_timer->start(timeout);
}
void QVPNCommand::timedOut(){
    //cancelTimeout();
    handleTimeout();
    finished();
    //this->deleteLater();
}
void QVPNCommand::cancelTimeout(){
    m_timer->stop();
}
void QVPNCommand::getResponse(){
    m_response = m_response + m_vpnProcess->readAllStandardOutput();
    if (m_response.contains(c_commandComplete)) {
        finished();
    }
    //handleResponse();
}
void QVPNCommand::commandSucceeded(){    
    emit success();
}
void QVPNCommand::commandFailed(){
    emit failed("Last " + m_lastCommand);
}
void QVPNCommand::finished(){
    cancelTimeout();
    //removeSpurii(m_response);

    if (m_response.length() > 0 ) {

        QStringList tempList = m_response.split("\n", Qt::SkipEmptyParts, Qt::CaseInsensitive);;
        m_responseList.clear();

        for (QString& string : tempList){
            removeSpurii(string);
            removeLeadingSpurii(string);
            if (string.length() > 0){
                QString str = "R: " + string;
                logSomething(str, c_responseColor);
                m_responseList.append(string);
            }
        }
    }
    handleResponse();
    if (m_responseList.length() == 1){
        emit m_callback->callback(m_responseList.at(0));
    } else {
        m_response.replace(c_commandComplete,"");
        emit m_callback->callback(m_response);
    }
    m_callback->deleteLater();

    this->deleteLater();
}
void QVPNCommand::removeSpurii(QString &response){
    if (response.contains("New feature"))
        response = "";

    response = response.remove(QRegularExpression("(\\r)")); // /(\\r)+/g
    response.replace("\t","");
    response.replace(":;",";");
    response.replace(c_commandComplete,"");
    response=response.trimmed();
}
void QVPNCommand::removeLeadingSpurii(QString &str){
    while (str.left(1).contains(QRegularExpression("[\\- /|\\\\]"))) {
        str.remove(0,1);
    }
    str=str.trimmed();
}
QString QVPNCommand::parseResponse(QString find, QString response){

    int p = response.indexOf(find);
    if (p == -1) return "";

    p = p + find.length();

    int e = response.indexOf("\n", p);
    if (e == -1) return "";

    QString result=response.mid(p, e-p).trimmed();

    return result;

}
void QVPNCommand::logSomething(QString str, QString color){
    if (m_logging) {
        emit log(str, color);
        staticFunctions::delay(1);
    }
}

