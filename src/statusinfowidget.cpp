#include "statusinfowidget.h"

#include "ui_statusinfowidget.h"


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
    ui->label_x->setNum(_1);
    ui->label_y->setNum(_2);
    ui->label_z->setNum(_3);
}
}