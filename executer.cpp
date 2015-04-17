#include "executer.h"
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QDebug>
//运行程序
void Executer::execute()
{
    execProcess = new QProcess ;
    //设置输入输出文件
    execProcess->setStandardInputFile(inputFilePath);
    execProcess->setStandardOutputFile(outputFilePath);
    execProcess->start(exePath);//开始运行
    //打印时间限制
    qDebug()<<"Time Limit : "<<timeLimit<<"ms";

    if(!execProcess->waitForStarted(timeLimit))
    {
        qDebug()<<"faild to start the program"<<exePath;
    }
    if(!execProcess->waitForFinished(timeLimit+500))
    {
        qDebug()<<"timeout";
        execProcess->kill();
    }

    //检查运行状态
    checkRunningStatus();
}
//运行状态
void Executer::checkRunningStatus()
{
    QProcess::ProcessError error ;
    error = execProcess->error();
    switch (error) {
    //无法运行
    case QProcess::FailedToStart:
        executeStatus = "Fail to start";
        break;
    //程序崩溃
    case QProcess::Crashed:
        executeStatus = "Runtime error";
        break;
    //运行超时
    case QProcess::Timedout:
        executeStatus = "Time Limit Exceeded";
        break;
    //无运行错误则继续
    case QProcess::UnknownError:
        qDebug()<<"running judge";
        checkAnswer();
        break;
    default:
        qDebug()<<"default";
        break;
    }
}
//检查结果
void Executer::checkAnswer()
{
    //获取输出结果和答案文件
    QFile ansFile(ansPath);
    QFile outputFile(outputFilePath);
    if(!ansFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"can't open file "<<ansPath;
    }
    if(!outputFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"can't open file "<<outputFilePath;
    }
    //比对答案与输出结果
    QVector<QString> ansVector ;
    QVector<QString> outputVector ;
    QString word ;
    //将单词放入容器中
    QTextStream ans(&ansFile);
    while(!ans.atEnd())
    {
        ans>>word;
        ansVector.push_back(word);
    }
    QTextStream out(&outputFile);
    while(!out.atEnd())
    {
        out>>word;
        outputVector.push_back(word);
    }
    //比较词汇个数
    if(ansVector.size()!= outputVector.size())
    {
        executeStatus = "Wrong Answer";
        qDebug("wrong size");
        qDebug()<<"ansvector size:"<<ansVector.size();
        qDebug()<<"outputVector size:"<<outputVector.size();
        return ;
    }
    else
    {
        int size = ansVector.size();
        int i ;
        for(i = 0 ; i<size ; i++)
        {
            if(ansVector[i]!=outputVector[i])
            {
                break;
            }
        }
        if(i < size )
        {
            executeStatus = "Wrong Answer";
            return ;
        }
    }
    if(!checkPresentation())
    {
        return ;
    }
    //通过验证
    executeStatus = "Accepted";
}
//检查格式
bool Executer::checkPresentation()
{
    QFile ansFile(ansPath);
    QFile outputFile(outputFilePath);
    if(!ansFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"can't open the file "<<ansPath;
    }
    if(!outputFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"can't open the file "<<outputFilePath;
    }
    //比较格式
    QTextStream ans(&ansFile);
    QTextStream out(&outputFile);
    QString line ;
    QString aString, oString ;
    while(!ans.atEnd())
    {
        line = ans.readLine();
        aString += line+"\n" ;
    }
    while(!out.atEnd())
    {
        line = out.readLine();
        oString += line + "\n";
    }
    if(aString != oString)
    {
        executeStatus = "Presentation Error";
        return false ;
    }
    else
    {
        return true;
    }
}

Executer::~Executer()
{

}

