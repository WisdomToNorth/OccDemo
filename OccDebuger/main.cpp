#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <qprocess.h>
#include <iostream>


#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "test_datastructure.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    KDebugger::MainWindowOcc w;
    w.show();

    return a.exec();

    //using namespace KDebugger;
    //KTest te;
    //te.testLineCross();
    //return 0;
}
