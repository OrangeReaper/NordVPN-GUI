#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H

#include <QString>
#include <QMetaEnum>
#include <QListWidget>

class staticFunctions
{
public:
    staticFunctions();
    static void delay(int mSecs, bool block=false);
    static bool confirm(QString title, QString question);
    static QStringList enumToStringList(QMetaEnum E);
};

#endif // STATICFUNCTIONS_H
