#ifndef SQL_CONFIG_H
#define SQL_CONFIG_H

#include <QWidget>
#include "mainwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Sql_config;
}

class Sql_config : public QWidget
{
    Q_OBJECT

public:
    explicit Sql_config(QWidget *parent = 0);
    ~Sql_config();

private:
    Ui::Sql_config *ui;
    //数据库连接
    QPushButton *quitBtn ;//离开按钮
    QPushButton *conBtn ;//连接数据库按钮
    QLabel *dbUserLb ;//数据库用户标签
    QLabel *dbPwdLb ;//数据库密码标签
    QLabel *dbNameLb ;//数据库名称标签
    QLabel *conStatuLb ;//数据库连接状态
    QLabel *dbAddressLb ;//数据库地址标签
    QLabel *dbPortLb ;//数据库端口标签
    QLineEdit *dbUserInput ;//数据库地址输入
    QLineEdit *dbPwdInput ;//数据库密码输入
    QLineEdit *dbNameInput ;//数据库名称输入
    QLineEdit *dbAddressInput ;//数据库地址输入
    QLineEdit *dbPortInput ;//数据库端口输入
    QSqlDatabase database ;//数据库链接
    MainWindow w ;//显示数据界面
private slots:
    //数据库连接
    void connect_db();
    void connect_quit();
};

#endif // SQL_CONFIG_H
