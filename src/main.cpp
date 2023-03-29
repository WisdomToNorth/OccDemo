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
//
//#include "test_datastructure.h"
//#include "gptools.h"
//#include <iostream>
//#include <gp_Pnt.hxx>
//using namespace KDebugger;
//
//int main(int argc, char* argv[])
//{
//    KTest te;
//
//    /* for (int i = 0; i < 1; ++i)
//     {
//         te.testKPnt2D(i);
//     }*/
//    te.testSameAngle();
//    return 0;
//}