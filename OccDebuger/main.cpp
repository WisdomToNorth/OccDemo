#include "MainWindowOcc.h"
#include <QtWidgets/QApplication>
#include <qprocess.h>
#include <iostream>


#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "lc_struct.h"

using namespace std;

int main(int argc, char* argv [])
{
    QApplication a(argc, argv);
    MainWindowOcc w;
    w.show();

    return a.exec();
}
