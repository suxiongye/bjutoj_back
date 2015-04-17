#ifndef COMPILER_H
#define COMPILER_H
#include <QString>
#include <QStringList>

class Compiler
{
public:
    //构造函数（确定编译器类型）
    Compiler(const QString &type="gcc"){compilerType = type;}
    //设置编译参数
    void setCompilerOption(const QStringList &option){compilerOption = option ;}
    //获取编译状态
    QString getStatus(){return compileStatus;}
    //获取编译错误
    QByteArray getErrorInfo(){return ErrorInfo;}
    //运行函数
    void execute();
    ~Compiler();
private:
    QString compilerType;
    QStringList compilerOption ;
    QString compileStatus ;
    QByteArray ErrorInfo;
};

#endif // COMPILER_H
