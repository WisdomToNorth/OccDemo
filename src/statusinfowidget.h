#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class StatusInfoWidgetClass;
};
QT_END_NAMESPACE
namespace KDebugger
{
class StatusInfoWidget : public QWidget
{
    Q_OBJECT

public:
    StatusInfoWidget(QWidget *parent = nullptr);
    ~StatusInfoWidget();
    void setMessage(const double &_1, const double &_2, const double &_3);

private:
    Ui::StatusInfoWidgetClass *ui;
};
} // namespace KDebugger
