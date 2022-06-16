#include "staticfunctions.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>
#include <QTime>

staticFunctions::staticFunctions(){

}
void staticFunctions::delay(int mSecs, bool block){
    QTime dieTime= QTime::currentTime().addMSecs(mSecs);
    while (QTime::currentTime() < dieTime)
        if (!block) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

bool staticFunctions::confirm(QString title, QString question){
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(question);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
      return true;

    return false;
}
QStringList staticFunctions::enumToStringList(QMetaEnum e){
    //QMetaEnum e = QMetaEnum::fromType<connectionType>();
    QStringList result;
    for (int k = 0; k < e.keyCount(); k++)
    {
         result.append(e.key(k));

    }
    return result;
}
