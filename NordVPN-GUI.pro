QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = nordvpn-gui
#QT += waylandcompositor

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classes/clsettings.cpp \
    classes/qcallback.cpp \
    classes/qlogger.cpp \
    classes/qnordvpnclient.cpp \
    classes/qoverlay.cpp \
    classes/qsettingsresult.cpp \
    classes/qtaskqueue.cpp \
    classes/staticconstants.cpp \
    classes/staticfunctions.cpp \
    commands/qanycommand.cpp \
    commands/qconnectcommand.cpp \
    commands/qcountriescommand.cpp \
    commands/qdisconnectcommand.cpp \
    commands/qgroupscommand.cpp \
    commands/qlogincommand.cpp \
    commands/qlogoutommand.cpp \
    commands/qsettingscommand.cpp \
    commands/qstatuscommand.cpp \
    commands/qvpncommand.cpp \
    dialogs/controlpanel.cpp \
    dialogs/dlgmanage.cpp \
    main.cpp \
    mainwindow.cpp \
    widgets/QCustomImage/qcustomimage.cpp \
    widgets/QCustomLabel/qcustomlabel.cpp \
    widgets/QCustomLineEdit/qcustomlineedit.cpp \
    widgets/QLabelCustomLabel/qlabelcustomlabel.cpp \
    widgets/QManagedItem/qmanageditem.cpp \
    widgets/QPasswordLineEdit/qpasswordlineedit.cpp \
    widgets/QSliderSwitch/qsliderswitch.cpp \
    widgets/QLabelSwitch/qlabelswitch.cpp

HEADERS += \
    classes/clsettings.h \
    classes/qcallback.h \
    classes/qlogger.h \
    classes/qnordvpnclient.h \
    classes/qoverlay.h \
    classes/qsettingsresult.h \
    classes/qtaskqueue.h \
    classes/staticconstants.h \
    classes/staticfunctions.h \
    commands/qgroupscommand.h \
    dialogs/controlpanel.h \
    dialogs/dlgmanage.h \
    mainwindow.h \
    commands/qanycommand.h \
    commands/qconnectcommand.h \
    commands/qcountriescommand.h \
    commands/qdisconnectcommand.h \
    commands/qlogincommand.h \
    commands/qlogoutommand.h \
    commands/qsettingscommand.h \
    commands/qstatuscommand.h \
    commands/qvpncommand.h \
    widgets/QCustomImage/qcustomimage.h \
    widgets/QCustomLabel/qcustomlabel.h \
    widgets/QCustomLineEdit/qcustomlineedit.h \
    widgets/QLabelCustomLabel/qlabelcustomlabel.h \
    widgets/QLabelSwitch/qlabelswitch.h \
    widgets/QManagedItem/qmanageditem.h \
    widgets/QPasswordLineEdit/qpasswordlineedit.h \
    widgets/QSliderSwitch/qsliderswitch.h

FORMS += \
    dialogs/controlpanel.ui \
    dialogs/dlgmanage.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    NordVPN-GUI.qrc \
    images.qrc \
    resources/fontawesome.qrc

DISTFILES += \
    LICENCE \
    README.md \
    currentFeatures.txt \
    history.txt \
    images/README.md \
    plannedFeatures.txt
