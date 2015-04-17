#include "compiler.h"
#include <QProcess>
#include <QDebug>
void Compiler::execute()
{
    QProcess compilerProcess; //编译进程
    compilerProcess.start(compilerType,compilerOption);//进行编译
    //编译无法开始
    if(!compilerProcess.waitForStarted())
    {
        qDebug("Failed to start the compiling process");
    }
    //编译超时
    if(!compilerProcess.waitForFinished())
    {
        qDebug("the compiling process maybe crashed");
    }
    //获取编译错误
    QByteArray result = compilerProcess.readAllStandardError();
    if(result.isEmpty())
    {
        qDebug("compiling ok");
        compileStatus = "Compile ok";
    }
    else
    {
        qDebug("Compile Error");
        compileStatus = "Compile Error";
        ErrorInfo = result ;
    }
}

Compiler::~Compiler()
{

}
