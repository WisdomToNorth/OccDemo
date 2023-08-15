#include <QtWidgets/QApplication>
#include <iostream>

#include "MainWindowOcc.h"
#include "lc_struct.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KDebugger::MainWindowOcc w;
    w.show();

    return a.exec();
}

// #include <queue>

// using namespace std;
// using namespace LeetCode;

// int main()
// {
//     return 0;
// }