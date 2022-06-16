#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *central_widget = new QWidget;
    QHBoxLayout *l_layout = new QHBoxLayout(central_widget);

    m_lineEdit = new QPasswordLineEdit(this);
    l_layout->addWidget(m_lineEdit);

    QLabel *myLabel = new QLabel();
    myLabel->setText("myLabel");
    l_layout->addWidget(myLabel);

    setCentralWidget(central_widget);
    show();

}

MainWindow::~MainWindow(){
    delete ui;
}

