#include "dlgmanage.h"
#include "ui_dlgmanage.h"

#include <QPushButton>
#include <QRegExpValidator>
#include <QMetaEnum>
#include <QSettings>
#include <QFontDatabase>
#include <QLabel>
#include <QDebug>
#include <QPainter>


#include "classes/staticfunctions.h"

dlgManage::dlgManage(QWidget *parent, manageMode mode, int maxEntries) :
    QDialog(parent),
    ui(new Ui::dlgManage)
{
    ui->setupUi(this);

    ui->pickTCPUDP->setVisible(mode == port);

    //*
    // Setup Add button (QCustomLabel)
    QImage source_image = QIcon(":/r/fontawesome-free-6.0.0-desktop/svgs/solid/square-plus.svg").pixmap(QSize(ui->addButton->width(),ui->addButton->height())).toImage();
    QImage new_image = source_image;
    //*
    //Change the colour
    QPainter painter(&new_image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(new_image.rect(), QColor(154, 205, 50));
    painter.end();
    //*
    //Set the image
    ui->addButton->setPixmap(QPixmap::fromImage(new_image));
    ui->addButton->setContentsMargins(0,0,0,0);
    //*
    //Set the cursor tp Pointing Hand
    m_cursor = QCursor(Qt::PointingHandCursor);
    ui->addButton->setCursor(m_cursor);
    //*
    //Initialise Signal
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addItem()));

    m_mode = mode;

    m_inputMask = "###.###.###.###"; // IPv4 Address Format
    m_placeholderText = "123.123.123.123";
    QString regExpStr = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    regExpStr = "^" + regExpStr
            + "\\." + regExpStr
            + "\\." + regExpStr
            + "\\." + regExpStr + "$";

    m_maxEntries=maxEntries;

    ui->warningLabel->setText("Items will be added/removed immediately!");
    ui->label->setVisible(m_mode == port);
    ui->lineEdit2->setVisible(m_mode == port);

    QRegExp ipRegex2;
    QRegExpValidator *validator2;

    switch (mode) {
      case dns:
        //default mode
        ui->warningLabel->setText("Maximum of 3 Entries allowed!");
        showCurrentDNSSetting();
        break;
      case subnet:
        hideButtons();
        populateListFromSetting("subnets");
        regExpStr.replace("$","\\/([1-9]|[12][0-9]|3[0-2])$");
        m_inputMask = "###.###.###.###/##"; // Subnet nomenclature
        m_placeholderText = "123.123.123.123/24";
        break;
    case port:
        hideButtons();
        populateListFromSetting("ports");
        regExpStr = "^(1|6[0-5][0-5][0-3][0-5]|[1-5][0-9][0-9][0-9][0-9]|[1-9][0-9]{0,3})$";
        m_inputMask ="#####"; // numbers in the range 1-65535
        m_placeholderText = "12345";
        ipRegex2.setPattern(regExpStr);
        validator2 = new QRegExpValidator(ipRegex2, this);
        ui->lineEdit2->setValidator(validator2);
        ui->lineEdit2->setInputMask(m_inputMask);
        break;
      default:
        // should never be reached
        break;
    }

    QRegExp ipRegex(regExpStr);
    QRegExpValidator *validator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit->setValidator(validator);        
    ui->lineEdit->setInputMask(m_inputMask);
    clearLineEdit();

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);


}

dlgManage::~dlgManage(){
    delete ui;
}
//*
//Slots
void dlgManage::addItem(){
    switch (m_mode) {
      case dns:
        //do nothing (Apply or OK will invoke the change)
        addItem(ui->lineEdit->text());
        break;
      case subnet:
        m_nordVPNClient->addWhitelistedSubnet(ui->lineEdit->text());
        addItem(ui->lineEdit->text());
        break;
    case port:
        m_nordVPNClient->addWhitelistedPort(ui->lineEdit->text(), ui->lineEdit2->text(), ui->pickTCPUDP->currentText());
        addItem(ui->lineEdit->text(), ui->lineEdit2->text(), ui->pickTCPUDP->currentText());
        break;
      default:
        // should never be reached
        break;
    }

    clearLineEdit();
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}
void dlgManage::deleteItem(){
    // get sender
    QManagedItem* item = qobject_cast<QManagedItem*>(sender());

    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        if (ui->listWidget->itemWidget(ui->listWidget->item(i)) == item ){

            bool confirmed=true;
            switch (m_mode) {
              case dns:
                //do nothing (Apply or OK will invoke the change)
                break;
              case subnet:
                confirmed=staticFunctions::confirm("Are You Sure?", "Subnet will be removed from whitelist immediately!");
                if (!confirmed) {
                    populateListFromSetting("subnets"); // Bodge
                } else {
                    m_nordVPNClient->removeWhitelistedSubnet(item->text());
                }
                break;
            case port:
                confirmed=staticFunctions::confirm("Are You Sure?", "Port will be removed from whitelist immediately!");
                if (!confirmed) {
                    populateListFromSetting("ports"); // Bodge
                } else {
                    m_nordVPNClient->removeWhitelistedPort(item->text());
                }
                break;
              default:
                // should never be reached
                break;
            }
            if (confirmed){
                QListWidgetItem* widgetItem = ui->listWidget->takeItem(i);
                delete widgetItem;
            }
            break;
        }
    }
    clearLineEdit();
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}
void dlgManage::inputChanged(QString input){

    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());

    if (inputIsInvalid(lineEdit)){
        // Allow lineEdit2 to be empty
        ui->addButton->setEnabled((lineEdit == ui->lineEdit2) && (lineEdit->text().toInt() == 0));
    } else {
        int pos = 0;
        ui->addButton->setEnabled(lineEdit->validator()->validate(input, pos) == QRegExpValidator::Acceptable);
    }
}
void dlgManage::apply(){
    switch (m_mode) {
      case dns:
        applyNewDNS();
      case subnet:
        break;
    case port:
        break;
      default:
        // should never be reached
        break;
    }
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}
void dlgManage::accept(){
    if (ui->buttonBox->button(QDialogButtonBox::Apply)->isEnabled())
        apply();

    QDialog::accept();
}
bool dlgManage::inputIsInvalid(QLineEdit *lineEdit){

    if (lineEdit == ui->lineEdit2){
        int i = ui->lineEdit2->text().toInt();
        if (i > 0){
            if (i <= ui->lineEdit->text().toInt())
            {
                return true;
            } else {
                return portEntryAlreadyListed();
            }
        }
    }
    QString inputMask = m_inputMask.replace("#","");
    QString input = lineEdit->text();
    if (lineEdit->text() == inputMask)
        return true;
    return entryAlreadyListed();
}
bool dlgManage::entryAlreadyListed(){
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        QWidget* widget = ui->listWidget->itemWidget(item);
        QManagedItem * managedItem = qobject_cast<QManagedItem*>(widget);
        if (managedItem->text() == ui->lineEdit->text())
            return true;
    }
    return false;
}
bool dlgManage::portEntryAlreadyListed(){
//    for(int i = 0; i < ui->listWidget->count(); ++i)
//    {
//        QListWidgetItem* item = ui->listWidget->item(i);
//        QWidget* widget = ui->listWidget->itemWidget(item);
//        QManagedItem * managedItem = qobject_cast<QManagedItem*>(widget);
//        if (managedItem->text() == ui->lineEdit->text())
//            return true;
//    }
    return false;
}

QString dlgManage::getModeAsString(){
    QMetaEnum metaEnum = QMetaEnum::fromType<manageMode>();
    QString result = metaEnum.valueToKey(m_mode);
    return result;
}
void dlgManage::showCurrentDNSSetting(){
    QSettings settings;
    m_currentSetting = settings.value(getModeAsString()).toString().split(",");
    ui->listWidget->clear();
    foreach (const QString &str, m_currentSetting) {
        if (str.isEmpty())
            break;

        addItem(str);
    }
}
void dlgManage::populateListFromSetting(QString setting){
    ui->listWidget->clear();
    QSettings settings;
    QStringList items = settings.value(setting).toString().split(",");
    foreach (const QString &item, items) {
        if (item.length() > 0) addItem(item);
    }
}
void dlgManage::addItem(QString str){
    QManagedItem* nextLabel = new QManagedItem();    
    QListWidgetItem *nextItem = new QListWidgetItem(ui->listWidget);
    nextItem->setFlags(Qt::ItemFlags());
    ui->listWidget->setItemWidget(nextItem, nextLabel);
    nextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    nextLabel->setText(str);
    connect(nextLabel, SIGNAL(buttonClicked()), this, SLOT(deleteItem()));
    ui->listWidget->sortItems();
}

void dlgManage::addItem(QString port1, QString port2, QString protocol){
    QString str = port1;
    if (port2.toInt() > 0)
        str = str + " - " + port2;

    str = str + " (" + protocol + ")";
    addItem(str.replace("/","|"));
}

void dlgManage::clearLineEdit(){
    ui->lineEdit2->setText(m_inputMask);
    ui->lineEdit->setText(m_inputMask);
    ui->lineEdit->setToolTip("Entry in the form: " + m_placeholderText);
    ui->lineEdit->setCursorPosition(0);
    ui->lineEdit->setEnabled(ui->listWidget->count() < m_maxEntries);
    staticFunctions::delay(1);
    ui->addButton->setEnabled(false);

}
void dlgManage::applyNewDNS(){
    QString dnsSetting = "";
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QManagedItem* item = qobject_cast<QManagedItem*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
        if (dnsSetting.length() > 0)
            dnsSetting = dnsSetting + ",";

        dnsSetting = dnsSetting + item->text();
    }
    QSettings settings;
    //m_currentSetting =
    settings.setValue(getModeAsString(), dnsSetting);
    m_currentSetting = dnsSetting.split(",");
    emit applyDNS(true);
}
void dlgManage::hideButtons(){
    ui->buttonBox->button(QDialogButtonBox::Apply)->setVisible(false);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
}
