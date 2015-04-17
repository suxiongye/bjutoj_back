#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "selecter.h"
#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *startButton ;
    QPushButton *stopButton ;
    QPushButton *quitButton ;
    Selecter *selecter ;
    Judger *judger ;
private slots:
    void startJudge();
    void stopJudge();

};

#endif // MAINWINDOW_H
