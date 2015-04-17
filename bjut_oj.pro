#-------------------------------------------------
#
# Project created by QtCreator 2015-03-28T11:52:08
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bjut_oj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    compiler.cpp \
    executer.cpp \
    selecter.cpp \
    sql_config.cpp

HEADERS  += mainwindow.h \
    compiler.h \
    executer.h \
    selecter.h \
    sql_config.h

FORMS    += mainwindow.ui \
    sql_config.ui
