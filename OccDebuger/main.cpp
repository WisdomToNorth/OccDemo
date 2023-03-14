#include <iostream>
#include <QtWidgets/QApplication>

#include "MainWindowOcc.h"
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    KDebugger::MainWindowOcc w;
    w.show();

    return a.exec();
}

//#include "test_datastructure.h"
//int main(int argc, char* argv[])
//{
//    KDebugger::KTest te;
//    te.testKPnt2D();
//    return 0;
//}