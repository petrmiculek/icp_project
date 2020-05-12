#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifndef NDEBUG
    srandom(time(NULL));
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
