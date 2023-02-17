#include "MainWindowOcc.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>
#include <thread>
#include <algorithm>

#include <qguiapplication.h>
#include <qscreen.h>
#include <qdebug.h>

#include "CADView.h"
#include "Ktimer.h"
#include "CustomQlistWidget.h"
#include "RobotLogger.h"
#include "global.h"

namespace KDebugger
{
MainWindowOcc::MainWindowOcc(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowOccClass())
{
    ui->setupUi(this);
    auto con = getUIConfig();
    this->setStyleSheet(G_GetUiStyleSheet(con));


    viewer_ = new CadView(this);

    unionset_ = new MultiUniset(viewer_);

    auto screenRect = QGuiApplication::screens();
    auto width = screenRect[0]->geometry().width();
    auto height = screenRect[0]->geometry().height();
    auto scwidth = width * 4 / 5;
    auto scheight = height * 4 / 5;
    this->setGeometry(width * 1 / 10, height / 10, scwidth, scheight);

    row_spin_ = new QSpinBox();
    col_spin_ = new QSpinBox();
    thread_spin_ = new QSpinBox();
    distance_spin_ = new QDoubleSpinBox();
    QLabel* label_row = new QLabel(" row");
    QLabel* label_col = new QLabel(" col");
    QLabel* label_thread = new QLabel(" thread");
    QLabel* label_dis = new QLabel(" distance");
    ui->mainToolBar->addWidget(label_row);
    ui->mainToolBar->addWidget(row_spin_);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(label_col);
    ui->mainToolBar->addWidget(col_spin_);
    ui->mainToolBar->addWidget(label_thread);
    ui->mainToolBar->addWidget(thread_spin_);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(label_dis);
    ui->mainToolBar->addWidget(distance_spin_);
    row_spin_->setAlignment(Qt::AlignRight);
    col_spin_->setAlignment(Qt::AlignRight);
    thread_spin_->setAlignment(Qt::AlignRight);
    distance_spin_->setAlignment(Qt::AlignRight);

    row_spin_->setMaximum(300);
    col_spin_->setMaximum(300);
    thread_spin_->setMinimum(0);
    thread_spin_->setMaximum(64);
    thread_spin_->setValue(0);
    row_spin_->setValue(300);
    col_spin_->setValue(300);
    distance_spin_->setValue(1.5);

    ui->gridLayout_view->addWidget(viewer_);
    iwCustomQListWidget* console_widget = ConsoleInit(this, this, false);
    ui->gridLayout_Console->addWidget(console_widget);

    QList<int> console_size;
    console_size << 3500 << 1000;
    ui->splitter->setSizes(console_size);
    ConsoleLog("Hello!");
    on_actionGenerate_triggered();
}

MainWindowOcc::~MainWindowOcc()
{
    delete unionset_;
    delete ui;
}

void MainWindowOcc::on_actionFitAll_triggered()
{
    viewer_->fitAll();
}


void MainWindowOcc::on_actionopt2_triggered()
{
    int def = 0;
    if (thread_spin_->value() > 0)
    {
        def = thread_spin_->value();
    }
    unionset_->multiCoreUnionSet(def);

}

void MainWindowOcc::on_actionview_triggered()
{
    unionset_->viewData();
}

void MainWindowOcc::on_actionGenerate_triggered()
{
    int rowcnt = row_spin_->value();
    int colcnt = col_spin_->value();
    double dis = distance_spin_->value();
    unionset_->reGenerateData(rowcnt, colcnt, dis);
}

void MainWindowOcc::on_actionOri_triggered()
{
    unionset_->badWay();
}

void MainWindowOcc::on_actionopt1_triggered()
{
    unionset_->oneCoreUnionSet();
}
}