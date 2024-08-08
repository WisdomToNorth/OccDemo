#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "iwConsole.h"
#include <iostream>

#define KLog iwConsole::print
#define KWarning iwConsole::warning
#define KError iwConsole::error

class QMainWindow;
class iwCustomQListWidget;
iwCustomQListWidget *InitConsole(QWidget *parentWidget = nullptr,
                                 QMainWindow *parentWindow = nullptr,
                                 bool redirectToStdOut = false);

#endif