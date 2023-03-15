#include "CustomQlistWidget.h"

#include <QApplication>
#include <QClipboard>
#include <QColor>
#include <QKeyEvent>


iwCustomQListWidget::iwCustomQListWidget(QWidget* parent)
    : QListWidget(parent)
{}

void iwCustomQListWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Copy))
    {
        int itemsCount = count();
        QStringList strings;
        for (int i = 0; i < itemsCount; ++i)
        {
            if (item(i)->isSelected())
            {
                strings << item(i)->text();
            }
        }

        QApplication::clipboard()->setText(strings.join("\n"));
    }
    else
    {
        QListWidget::keyPressEvent(event);
    }
}
