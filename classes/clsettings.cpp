/*
 * This generic class initialises a QSettings object from a JSON File
 *
 * It will:
 *      Populate QSettings with default values contained in the JSON file
 *      when the QSettings object does not already contain that setting.
 *
 *      Remove any setting which is contained in the QSettings object where
 *      that setting is not contained in the JSON file.
 *
 * To use this class simply call the static function validate(),
 *
 *      eg: clSettings::validate();
 *
 * Example JSON file
 *
 *      {
 *        "settings":{
 *          "setting1":"value1",
 *          "setting2":"value2",
 *          "setting3":"value3"
 *        }
 *      }
 *
 */
#include "clsettings.h"
#include <QSettings>
#include <QFile>
#include <QJsonDocument>
#include <QString>
#include <QJsonObject>
#include <QStringList>

clSettings::clSettings()
{

}
void clSettings::validate(){
    QFile defaultSettings(":/r/resources/defaultSettings.json");
    defaultSettings.open(QIODevice::ReadOnly);

    QJsonDocument jDoc;

    QString jString=QString::fromUtf8(defaultSettings.readAll()).simplified();
    jDoc=QJsonDocument::fromJson(jString.toUtf8());

    QJsonObject jObj;
    jObj=jDoc.object();

    QJsonObject jSettings=jObj["settings"].toObject();

    QSettings settings;
    QStringList keys = settings.allKeys();

    //Ensure that an entry exists in QSettings for each Setting listed in JSON file
    QJsonObject::iterator it;
    QStringList jsonKeys;
    for(it=jSettings.begin();it!=jSettings.end();it++){
        jsonKeys << it.key();
        if (!keys.contains(it.key())){
            settings.setValue(it.key(), it.value().toString());
        }
    }

    //Ensure that all keys in QSettings are contained in the JSON
    QStringList deleteKeys;
    for(QString &key : settings.allKeys()){
        if (!key.contains("/names/")){
            if (!jsonKeys.contains(key)){
                deleteKeys << key; //missing from JSON file... so delete
            }
        }
    }
    //Now delete those keys that were not found in the json file
    for(QString &key : deleteKeys){
        settings.remove(key);
    }

    defaultSettings.close();
}
void clSettings::reset(){
    QSettings settings;
    settings.clear();
    validate();
}
