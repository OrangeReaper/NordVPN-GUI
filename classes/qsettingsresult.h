#ifndef QSETTINGSRESULT_H
#define QSETTINGSRESULT_H

#include <QString>

class QSettingsResult
{
public:
    QSettingsResult();
    bool hasResult()        { return m_hasResult; }
    bool boolResult()       { return m_bool; }
    QString stringResult()  { return m_string; }

    void setHasResult(bool result)      { m_hasResult = result; }
    void setStringResult(QString string){ m_string = string; }
    void setBoolResult(bool value)      { m_bool = value; }
private:
    bool    m_hasResult    = false;
    bool    m_bool         = false;
    QString m_string       = "";


};

#endif // QSETTINGSRESULT_H
