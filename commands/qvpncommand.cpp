#include "qvpncommand.h"
#include <QDebug>
#include <QRegularExpression>

#include "classes/staticfunctions.h"

QVPNCommand::QVPNCommand(QString command, int timeout, QCallback *callback, QObject *parent) : QObject(parent){
    m_callback=callback;
    m_command=command;
    m_timeout=timeout;
}
QVPNCommand::~QVPNCommand(){
    //qDebug() << "QVPNCommand Destroyed: " + m_command;
}
void QVPNCommand::handleResponse(){
    finished();
}
void QVPNCommand::handleTimeout(){
    commandFailed();
}
void QVPNCommand::sendCommand(){
    connect(this, SIGNAL(log(QString,QString)), QLogger::getInstance(), SLOT(logSomething(QString,QString)));

    // Prepare command
    QString str = "C: " + m_command;
    logSomething(str, c_commandColor);
    m_lastCommand = str;

    QProcess * pnProcess = new QProcess();
    pnProcess->start("/bin/sh", QStringList() << "-c" << m_command);
    if (!pnProcess->waitForFinished(m_timeout)){
        //Process timed out
        QString errorMessage="E:" + m_lastCommand + ": timed out";
        logSomething(errorMessage, c_errorColor);
    } else {
        // process finished
        m_response = pnProcess->readAllStandardOutput();

        if (pnProcess->exitStatus() != 0) {
            // The process failed.
            QString errorMessage = pnProcess->errorString();
            errorMessage="E:" + m_lastCommand + ": failed\n" + "E:" + errorMessage;
            logSomething(errorMessage, c_errorColor);
        }
    }

    pnProcess->deleteLater();
    handleResponse();

}

void QVPNCommand::commandSucceeded(){    
    emit success();
}
void QVPNCommand::commandFailed(){
    emit failed("Last " + m_lastCommand);
}
void QVPNCommand::finished(){
    if (response().length() > 0){
        QString str = "R: " + response();
        logSomething(str, c_responseColor);
    }

    emit m_callback->callback(m_response);
    m_callback->deleteLater();

    this->deleteLater();
}

void QVPNCommand::removeSpurii(QString &response){
//    if (response.contains("New feature"))
//        response = "";

    response = response.remove(QRegularExpression("(\\r)")); // /(\\r)+/g
    response.replace("\t","");
    response.replace(":;",";");
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
    removeSpurii(str);
    removeLeadingSpurii(str);
    if (m_logging) {
        emit log(str, color);
        staticFunctions::delay(1);
    }
}

