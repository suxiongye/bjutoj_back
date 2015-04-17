#ifndef SELECTER_H
#define SELECTER_H
#include <QThread>

extern bool isJudgeActivated ;
//将数据库程序选出，放入环形缓冲区
class Selecter:public QThread
{
public:
   void run();
};
//对程序进行判别
class Judger:public QThread
{
public :
    void run();
};
//删除可能出现的错误报告
class KillWrong:public QThread
{
public :
    void run();
};

#endif // SELECTER_H
