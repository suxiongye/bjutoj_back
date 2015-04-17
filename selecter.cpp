#include "selecter.h"
#include "executer.h"
#include "compiler.h"
#include <QtSql>
#include <QSemaphore>
#include <QSqlQuery>
#include <QTime>

const int BufferSize = 50 ;
int buffer[BufferSize];
bool isJudgeActivated = false ;
//申请50个空余信号量
QSemaphore freeBuffer(BufferSize);
//申请0个已使用信号量
QSemaphore usedBuffer;

//评判函数
void judge(int id)
{
    QSqlQuery query ;
    QString tempString ;
    qDebug()<<"id:"<<id;
    //获取使用的编程语言
    query.prepare("select compiler from status where runID = :id");
    query.bindValue(":id",id);
    query.exec();
    query.first();
    qDebug()<<query.value(0).toString();
    //编程语言及编译器
    QString languageName = query.value(0).toString();

    QString compilerName ;
    //设置编译选项
    QStringList opt ;
    if(languageName == "C")
    {
        compilerName = "gcc";
        //设置编译输入输出
        opt<<"-w"<<"-o"<<"E:/qt_test/coreLab/exe/temp.exe";
        tempString = "E:/qt_test/coreLab/code/";
        tempString.append(QString::number(id));
        tempString.append(".c");
        opt << tempString ;
    }
    else
    {

    }
    //进行编译
    Compiler compiler(compilerName);
    compiler.setCompilerOption(opt);
    compiler.execute();
    tempString = compiler.getStatus();
    if(tempString == "Compile Error")
    {
        query.prepare("update status set judgeStatus = 'Compile Error',"
                      "remarks=:rem where runID= :rid");
        query.bindValue(":rem",compiler.getErrorInfo());
        query.bindValue(":rid",id);
        query.exec();
        return ;
    }
    //运行评判
    Executer executer ;
    //找到problemID
    query.prepare("select problemID from status where runID= :rid");
    query.bindValue(":rid",id);
    query.exec() ;
    query.first();
    QString pidString = query.value(0).toString();
    //设置输入输出文件
    tempString = "E:/qt_test/coreLab/input/";
    tempString.append(pidString);
    tempString.append(".txt");
    executer.setInputFile(tempString);
    tempString = "E:/qt_test/coreLab/output/";
    tempString.append(QString::number(id));
    tempString.append(".txt");
    executer.setOutputFile(tempString);
    //设置执行程序及答案路径
    tempString = "E:/qt_test/coreLab/exe/temp.exe";
    executer.setExePath(tempString);
    tempString = "E:/qt_test/coreLab/answer/";
    tempString.append(pidString);
    tempString.append(".txt");
    executer.setAnsPath(tempString);
    //设置时间内存限制
    executer.setTimeLimit();
    executer.setMemoryLimit();
    //开始评判
    executer.execute();
    qDebug()<<executer.getStatus();

    //更新状态
    const QTime startTime(11,30,0);
    query.prepare("update status set judgeStatus = :js where runID = :rid");
    query.bindValue(":js",executer.getStatus());
    query.bindValue(":rid",id);
    query.exec();
    //如果通过评判，更新排名
    if(executer.getStatus()=="Accepted")
    {
        qDebug()<<"Accepted,updating the database\n";
        //更新题目通过数及通过率
        query.prepare("update problemLib set accepted = accepted+1,ratio=accepted/submited"
                      " where pid = :pid");
        query.bindValue(":pid",pidString.toInt());
        query.exec();
        //将通过评判的作者和时间加入排名榜
        QString team ;
        QTime submitTime ;
        query.prepare("select author,submitTime from status where runID = :rid");
        query.bindValue(":rid",id);
        query.exec();
        //取出名字和提交时间
        while(query.next())
        {
            team = query.value(0).toString();
            submitTime = query.value(1).toTime();
        }
        //取出人物解决问题数，解决时间，对应问题通过时间，对应问题提交时间
        QString queryString ;
        queryString.append("select solvedNumber,contestTime,");
        queryString.append("passTime_"+pidString);
        queryString.append(",");
        queryString.append("submitTimes_"+pidString);
        queryString.append(" from rankList where team = '");
        queryString.append(team);
        queryString.append("'");
        qDebug()<<queryString;
        query.exec(queryString);

        QTime passTime,contestTime;
        int submitNum = 0 ;
        int solvedNum = 0 ;
        int temp ;
        //存储到变量中
        while(query.next())
        {
            solvedNum = query.value(0).toInt();
            contestTime = query.value(1).toTime();
            passTime = query.value(2).toTime();
            submitNum = query.value(3).toInt();
        }
        solvedNum+=1 ;
        submitNum+=1 ;
        //计算时间
        temp = startTime.secsTo(QTime(0,0,0));
        submitTime = submitTime.addSecs(temp);
        temp = (-submitNum)*1200 ;//除去系统等待时间
        passTime = submitTime.addSecs(temp) ;
        //更新时间
        temp = passTime.secsTo(QTime(0,0,0));
        contestTime = contestTime.addSecs(-temp);
        qDebug()<<"contest Time:"<<contestTime ;
        qDebug()<<"solved number: "<<solvedNum ;
        qDebug()<<"submit number: "<<submitNum ;
        //更新数据库
        QString updateString ;
        updateString = QString("update rankList set solvedNumber = %1,"
                               "contestTime=\'").arg(solvedNum);
        updateString.append(contestTime.toString("HH:mm:ss"));
        updateString.append("\',passTime_"+pidString);
        updateString.append("=\'");
        updateString.append(passTime.toString("HH:mm:ss"));
        updateString.append("\',submitTimes_"+pidString);
        updateString.append("=");
        QString str ;
        str.setNum(submitNum);
        updateString.append(str);
        updateString.append(" where team=\'");
        updateString.append(team);
        updateString.append("\'");
        qDebug()<<"updateString:"<<updateString;
        query.exec(updateString);
    }

}
//关闭错误类
void KillWrong::run()
{
    QProcess killProcess ;
    while(isJudgeActivated)
    {
        //im--image name镜像名,-f 强制终止进程
        killProcess.execute("taskkill /f /im dwwin.exe");
        msleep(1000);
    }
}


//选出题目类
void Selecter::run()
{
    int i = 0 ;
    while(isJudgeActivated)
    {
        //选出所有处于等待状态的代码
        QSqlQuery query("select runID from status where judgeStatus='Waiting'");
        int index = 0 ;
        //选出所有未被评判的题目，放入缓冲区
        while(query.next())
        {
            index = query.value(0).toInt();
            //申请一个空闲信号量
            freeBuffer.acquire();
            //把运行编号加入缓冲区
            buffer[i%BufferSize] = index ;
            QSqlQuery q;
            q.prepare("update status set judgeStatus = 'Judging' where runID=:rid");
            q.bindValue(":rid",index);
            q.exec();
            ++i ;
            //已使用的信号量加一
            usedBuffer.release();
        }
        msleep(500);
    }
}
//评判类
void Judger::run()
{
    int i = 0 ;
    while(isJudgeActivated)
    {
        //选取一个题目进行评判
        int id ;
        usedBuffer.acquire();
        id = buffer[i % BufferSize];//从缓冲区中读取运行id
        ++i ;
        qDebug()<<"runID"<<id ;
        freeBuffer.release();
        //进行评判
        judge(id);
    }
}
