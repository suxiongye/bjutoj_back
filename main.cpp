#include <QApplication>
#include "sql_config.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sql_config sql_config;
    sql_config.show();
    return a.exec();
}
