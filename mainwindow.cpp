#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //开始结束按钮
    startButton = new QPushButton("start",this);
    startButton->setGeometry(QRect(20,20,150,30));
    stopButton = new QPushButton("stop",this);
    stopButton->setGeometry(QRect(180,20,150,30));
    //链接函数
    connect(startButton,SIGNAL(clicked()),this,SLOT(startJudge()));
    connect(stopButton,SIGNAL(clicked()),this,SLOT(stopJudge()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
//开始评判函数
void MainWindow::startJudge()
{
    //判断是否开始
    if(isJudgeActivated)
    {
        return ;
    }
    isJudgeActivated = true ;
    selecter = new Selecter ;
    judger = new Judger ;
    selecter->start();
    judger->start();
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
}
//结束评判函数
void MainWindow::stopJudge()
{
    if(!isJudgeActivated)
    {
        return ;
    }
    isJudgeActivated = false ;
    selecter->quit();
    judger->quit();
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}
//退出函数

