#include <iostream>
#include "mainwindow.h"
#include <QApplication>
#include "bnode.h"
#include <cstdlib>
#include <cstring>
#include "srtl-driver.hh"
#include "util.h"

using namespace std;
using namespace util;

int main(int argc, char *argv[])
{
//    char *args[20];
//    args[0] = strdup("srtl");
//    args[1] = strdup("-s");
//    args[2] = strdup("-o");
//    args[3] = strdup("/home/sushant/qt/gccproj/out.txt");
//    args[4] = strdup("/home/sushant/qt/gccproj/i386.specmd");
//    util::ThreadedParser obj(5, args);
//    obj.start();
//    obj.wait();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showMaximized();
    return a.exec();
}
