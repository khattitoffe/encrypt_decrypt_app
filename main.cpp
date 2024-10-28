#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

void createBackup(QString filepath)
{

    //if(copyFile(filepath))
}



