/****************************************************************************
** Copyright 2022 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "iwConsole.h"
#include <iostream>

#define ConsoleLog iwConsole::print
#define Cout std::cout<<'['<<__FILE__<<':'<<__LINE__<<']'

class QMainWindow;
class iwCustomQListWidget;
iwCustomQListWidget* ConsoleInit(QWidget* parentWidget = nullptr,
    QMainWindow* parentWindow = nullptr,
    bool redirectToStdOut = false);

#endif