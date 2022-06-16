#ifndef DLGMANAGE_H
#define DLGMANAGE_H

#include <QDialog>
#include <QListWidget>

#include "widgets/QManagedItem/qmanageditem.h"
#include "widgets/QCustomLabel/qcustomlabel.h"
#include "classes/qnordvpnclient.h"

namespace Ui {
class dlgManage;
}

class dlgManage : public QDialog
{
    Q_OBJECT

public:
    enum manageMode { dns, subnet, port };
    Q_ENUM(manageMode);

    explicit dlgManage(QWidget *parent = nullptr, manageMode mode=dns, int maxEntries=3);
    ~dlgManage();

public slots:
    void inputChanged(QString input);
    void addItem();
    void deleteItem();
    void apply();
    void accept();

signals:
    void applyDNS(bool enable);

private:
    Ui::dlgManage *ui;

    void clearLineEdit();
    void showCurrentDNSSetting();
    void populateListFromSetting(QString setting);
    void addItem(QString str);
    void addItem(QString port1, QString port2, QString protocol);
    void applyNewDNS();
    void hideButtons();

    bool inputIsInvalid(QLineEdit *lineEdit);
    bool entryAlreadyListed();
    bool portEntryAlreadyListed();

    QString m_inputMask;
    QString m_placeholderText;
    QString getModeAsString();
    QStringList m_currentSetting;

    QCursor m_cursor;

    manageMode m_mode;

    int m_maxEntries;

    QNordVPNClient  *m_nordVPNClient=QNordVPNClient::getInstance();

};

#endif // DLGMANAGE_
