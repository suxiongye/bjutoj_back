#include "sql_config.h"
#include "ui_sql_config.h"
#include <QMessageBox>

Sql_config::Sql_config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sql_config)
{
    ui->setupUi(this);
    this->setMaximumSize(400,600);
    this->setMinimumSize(400,400);
    //标题
    this->setWindowTitle("Database connect");
    //connect interface
    QLabel *mysql = new QLabel("MySQL",this);
    mysql->setGeometry(QRect(170,20,300,30));
    dbUserLb = new QLabel("DB User",this);
    dbUserInput = new QLineEdit("root",this);
    dbPwdLb = new QLabel("DB Pwd",this);
    dbPwdInput = new QLineEdit("root",this);
    dbUserLb->setGeometry(QRect(20,60,80,30));
    dbUserInput->setGeometry(QRect(100,60,200,30));
    dbPwdLb->setGeometry(QRect(20,100,80,30));
    dbPwdInput->setGeometry(QRect(100,100,200,30));
    dbPwdInput->setEchoMode(QLineEdit::Password);
    dbNameLb = new QLabel("DB Name",this);
    dbNameLb->setGeometry(QRect(20,140,200,30));
    dbNameInput = new QLineEdit("oj",this);
    dbNameInput->setGeometry(QRect(100,140,200,30));
    dbAddressLb = new QLabel("DB Address",this);
    dbAddressLb->setGeometry(QRect(20,180,200,30));
    dbAddressInput = new QLineEdit("localhost",this);
    dbAddressInput->setGeometry(QRect(100,180,200,30));
    dbPortLb = new QLabel("DB Port",this);
    dbPortLb->setGeometry(QRect(20,220,200,30));
    dbPortInput = new QLineEdit("3306",this);
    dbPortInput->setGeometry(QRect(100,220,200,30));
    conBtn = new QPushButton("connect",this);
    quitBtn = new QPushButton("quit",this);
    conBtn->setGeometry(QRect(20,300,140,30));
    quitBtn->setGeometry(QRect(170,300,140,30));
    connect(conBtn,SIGNAL(clicked()),this,SLOT(connect_db()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
    conStatuLb = new QLabel("Connect status: no connect ",this);
    conStatuLb->setGeometry(QRect(50,260,200,30));
}

Sql_config::~Sql_config()
{
    delete ui;
}
//数据库连接函数
void Sql_config::connect_db()
{
    //判断数据库不为空
    if(dbUserInput->text() == "" || dbPwdInput->text()==""||dbNameInput->text()=="")
    {
        QMessageBox::warning(this,qApp->tr("warning"),qApp->tr("Can accept blank"),QMessageBox::Ok);
        return ;
    }
    if(database.isValid())
    {
        //若已经添加驱动则不操作
    }
    else
    {
        //连接数据库驱动
        database = QSqlDatabase::addDatabase("QMYSQL");
    }
    database.setDatabaseName(dbNameInput->text());
    database.setHostName(dbAddressInput->text());
    database.setPassword(dbPwdInput->text());
    database.setUserName(dbUserInput->text());
    database.setPort(dbPortInput->text().toInt());

    if(!database.open())
    {
        QMessageBox::warning(this,"warning","Can not connect to database ");
        return ;
    }
    //隐藏其他按钮
    dbUserInput->setEnabled(false);
    dbPwdInput->setEnabled(false);
    dbPortInput->setEnabled(false);
    dbNameInput->setEnabled(false);
    dbAddressInput->setEnabled(false);
    conStatuLb->setText("Status : Connecting to "+dbNameInput->text());
    conBtn->setText("disconnect");
    disconnect(conBtn,SIGNAL(clicked()),this,SLOT(connect_db()));
    connect(conBtn,SIGNAL(clicked()),this,SLOT(connect_quit()));
    //连接成功后显示w2
    w.show();
    w.setWindowTitle(dbNameInput->text());
}
void Sql_config::connect_quit()
{
    database.close();
    disconnect(conBtn,SIGNAL(clicked()),this,SLOT(connect_quit()));
    conStatuLb->setText("Connect status: no connect .");
    //隐藏其他按钮
    dbUserInput->setEnabled(true);
    dbPwdInput->setEnabled(true);
    dbPortInput->setEnabled(true);
    dbNameInput->setEnabled(true);
    dbAddressInput->setEnabled(true);
    conBtn->setText("connect");
    connect(conBtn,SIGNAL(clicked()),this,SLOT(connect_db()));
    w.close();
}
