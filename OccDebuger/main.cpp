#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    KDebugger::MainWindowOcc w;
    w.show();

    return a.exec();

}
