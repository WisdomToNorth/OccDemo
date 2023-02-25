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
#include <qspinbox.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "CADView.h"
#include "Ktimer.h"
#include "CustomQlistWidget.h"
#include "KLogger.h"
#include "global.h"
#include "MultiUniset.h"
#include "DataGenerator.h"
#include "kd_search.h"

#include "ui_MainWindowOcc.h"


namespace KDebugger
{
MainWindowOcc::MainWindowOcc(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowOccClass()),
    unionset_(nullptr),
    kdtree_(nullptr)

{
    ui->setupUi(this);
    auto con = getUIConfig();
    this->setStyleSheet(G_GetUiStyleSheet(con));


    viewer_ = new CadView(this);
    G_Context = viewer_->getContext();

    data_generator_ = new DataGenerator(viewer_);
    ui->gridLayout_view->addWidget(viewer_);

    setUpUI();
    ConsoleLog("Hello!");
    on_pb_generate_pressed();
}

MainWindowOcc::~MainWindowOcc()
{
    if (data_generator_)delete data_generator_;
    if (unionset_)delete unionset_;

    delete ui;
}

void MainWindowOcc::setUpUI()
{
    auto screenRect = QGuiApplication::screens();
    auto width = screenRect[0]->geometry().width();
    auto height = screenRect[0]->geometry().height();
    auto scwidth = width * 4 / 5;
    auto scheight = height * 4 / 5;
    this->setGeometry(width * 1 / 10, height / 10, scwidth, scheight);

    iwCustomQListWidget* console_widget = ConsoleInit(this, this, false);
    ui->gridLayout_Console->addWidget(console_widget);

    QList<int> console_size;
    console_size << 3500 << 1000;
    ui->splitter->setSizes(console_size);
    QList<int> console_size_vert;
    console_size_vert << 600 << 3000;
    ui->splitter_2->setSizes(console_size_vert);
}

void MainWindowOcc::on_actionFitAll_triggered()
{
    viewer_->fitAll();
}

void MainWindowOcc::on_actionview_triggered()
{
    data_generator_->viewData();
}

//unionfind
void MainWindowOcc::on_act_unionfind_ori_triggered()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    unionset_->badWay();
}
//unionfind
void MainWindowOcc::on_act_unionfind_opt1_triggered()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    unionset_->oneCoreUnionSet();
}
//unionfind
void MainWindowOcc::on_act_unionfind_opt2_triggered()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    int def = 0;
    if (ui->sb_core->value() > 0)
    {
        def = ui->sb_core->value();
    }
    unionset_->multiCoreUnionSet(def);
}

void MainWindowOcc::on_actionkd_find1D_triggered()
{
    if (!kdtree_)kdtree_ = new KDTree(data_generator_);
    kdtree_->getOneDRange(ui->dsb_left->value(), ui->dsb_right->value());
}

void MainWindowOcc::on_actionori_find1D_triggered()
{
    if (!kdtree_)kdtree_ = new KDTree(data_generator_);
    kdtree_->getOneDRangeOri(ui->dsb_left->value(), ui->dsb_right->value());
}
void MainWindowOcc::on_actionkd_find2D_triggered()
{
    if (!kdtree_)kdtree_ = new KDTree(data_generator_);

}
void MainWindowOcc::on_actionori_find2D_triggered()
{
    if (!kdtree_)kdtree_ = new KDTree(data_generator_);

}
void MainWindowOcc::on_pb_valueMax_pressed()
{
    ui->sb_col->setValue(300);
    ui->sb_row->setValue(300);
}
void MainWindowOcc::on_pb_valueSmall_pressed()
{
    ui->sb_col->setValue(5);
    ui->sb_row->setValue(5);
}

void MainWindowOcc::on_pb_generate_pressed()
{
    DataParameter param;
    param.colcnt = ui->sb_col->value();
    param.rowcnt = ui->sb_row->value();
    param.seed = ui->sb_seed->value();

    param.dis = ui->dsb_distance->value();
    param.precision = ui->sb_precision->value();
    param.w_h_same = ui->cb_wh_same->isChecked();
    param.rand_loc = ui->cb_rand_loc->isChecked();
    param.rand_size = ui->cb_rand_size->isChecked();

    if (!param.rand_loc)
    {
        param.defaultpar.rand_loc_min = 1.0;
        param.defaultpar.rand_loc_max = 1.0;
    }
    if (!param.rand_size)
    {
        param.defaultpar.rand_size_min = 1.0;
        param.defaultpar.rand_size_max = 1.0;
    }

    data_generator_->reGenerateData(param);
}


}