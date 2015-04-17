#ifndef EXECUTER_H
#define EXECUTER_H
#include <QString>
#include <QProcess>
class Executer
{
public:
    Executer(){}
    //设置输入用例
    void setInputFile(const QString &file){inputFilePath = file ;}
    //设置输出结果
    void setOutputFile(const QString &file){outputFilePath = file ;}
    //设置运行程序
    void setExePath(const QString &path){exePath = path;}
    //设置标准答案路径
    void setAnsPath(const QString &ans){ansPath = ans;}
    //设置时间限制
    void setTimeLimit(int limit = 1000){timeLimit = limit;}
    //设置内存限制
    void setMemoryLimit(int limit = 32){memoryLimit = limit ;}
    //获取评判状态
    QString getStatus(){return executeStatus;}
    //运行获取结果
    void execute();
    //检查运行状态
    void checkRunningStatus();
    //检查格式
    bool checkPresentation();
    //进行评判
    void checkAnswer();
    ~Executer();
private:
    QString inputFilePath ;
    QString outputFilePath;
    QString exePath;
    QString ansPath;
    int timeLimit ;
    int memoryLimit;
    QString executeStatus;
    QProcess *execProcess;

};

#endif // EXECUTER_H
