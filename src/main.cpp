#include <iostream>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     KDebugger::MainWindowOcc w;
//     w.show();

//     return a.exec();
// }

#include "datautils.h"

using namespace rfbase;

int main()
{
    std::ignore = rfbase::DataUtils::makeDir("test");
    return 0;
}