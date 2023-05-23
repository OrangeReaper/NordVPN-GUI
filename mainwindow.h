#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include "classes/qnordvpnclient.h"
#include "classes/qlogger.h"
#include "widgets/QCustomImage/qcustomimage.h"
#include "dialogs/controlpanel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static const QString s_version;
protected:
      void mousePressEvent(QMouseEvent *event);
      void mouseMoveEvent(QMouseEvent *event);

public slots:
//    void enable();
//    void disable();
    void controlPanel();
    void userClose();
    void VPNChangedState(int);

private:
    Ui::MainWindow *ui;

    void saveWindowState();

    QCustomImage *pgmImage;

    QPoint m_pos; // For moving window without titlebar

    QAction *a_disable;
    QAction *a_enable;
    QAction *a_controlPanel;
    QAction *a_quit;
    QAction *a_disQuit;

    QNordVPNClient *m_nordVPNClient=0;
    QLogger *m_logger=0;
    ControlPanel *m_controlPanel=0;

};
#endif // MAINWINDOW_H
