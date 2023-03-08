#include "KLogger.h"
#include "CustomQlistWidget.h"


class RobotMainWindow;

iwCustomQListWidget* InitConsole(QWidget* parentWidget,
    QMainWindow* parentWindow, bool redirectToStdOut)
{
    iwCustomQListWidget* console_widget = new iwCustomQListWidget(parentWidget);
    iwConsole::init(console_widget, parentWidget, parentWindow, redirectToStdOut);
    return console_widget;

}
