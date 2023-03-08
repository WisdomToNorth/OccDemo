
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "iwConsole.h"
#include <iostream>

#define ConsoleLog iwConsole::print
#define Cout std::cout<<'['<<__FILE__<<':'<<__LINE__<<']'

class QMainWindow;
class iwCustomQListWidget;
iwCustomQListWidget* InitConsole(QWidget* parentWidget = nullptr,
    QMainWindow* parentWindow = nullptr,
    bool redirectToStdOut = false);

#endif