#include "statusinfowidget.h"

#include "ui_statusinfowidget.h"

#include <QPalette>

namespace KDebugger
{
StatusInfoWidget::StatusInfoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StatusInfoWidgetClass())
{
    ui->setupUi(this);
}

StatusInfoWidget::~StatusInfoWidget()
{
    delete ui;
}
void StatusInfoWidget::setMessage(const double& _1, const double& _2, const double& _3)
{
    // QLabel* la;
    ui->label_x->setText(QString::number(_1, 'f', 2));
    ui->label_y->setText(QString::number(_2, 'f', 2));
    ui->label_z->setText(QString::number(_3, 'f', 2));
}
}