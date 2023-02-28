#include "MainWindowOcc.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>
#include <thread>
#include <algorithm>

#include <qguiapplication.h>
#include <QScreen>
#include <qdebug.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <QStatusBar>
#include <QMenu>
#include <QPointer>
#include <QDesktopWidget>

#include "CADView.h"
#include "Ktimer.h"
#include "CustomQlistWidget.h"
#include "KLogger.h"
#include "global.h"
#include "MultiUniset.h"
#include "DataGenerator.h"
#include "kd_search.h"
#include "K_Pnt.h"
#include "ui_MainWindowOcc.h"
#include "statusinfowidget.h"
#include "linedrawer.h"
#include "PreLine.h"

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


    cadview_ = new CadView(this);
    G_Context = cadview_->getContext();
    cadview_->moveInfoCb = std::bind(&MainWindowOcc::handleMouseMove,
        this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    cadview_->leftClickCb = std::bind(&MainWindowOcc::handleLeftPress,
        this, std::placeholders::_1, std::placeholders::_2);
    cadview_->rightClickCb = std::bind(&MainWindowOcc::handleRightPress,
        this, std::placeholders::_1);


    info_widget_ = new StatusInfoWidget(this);
    ui->statusBar->addPermanentWidget(info_widget_);

    data_generator_ = new DataGenerator(cadview_);
    ui->gridLayout_view->addWidget(cadview_);

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
void MainWindowOcc::on_action_drawline_triggered()
{
    curmode_ = AppModeEnum::draw_line;
}
void MainWindowOcc::on_actionendCPU_triggered()
{
    G_Stop_Program = true;
}
void MainWindowOcc::on_action_cpuline_triggered()
{
    const std::vector<KBox>& context_info = data_generator_->getData();
    for (auto& obj : preline_vec_)
    {
        K_Timer timer;
        obj->reGenerate(context_info);
        timer.timeFromBegin();
    }

}

void MainWindowOcc::on_action_normline_triggered()
{
    for (auto& obj : preline_vec_)
    {
        K_Timer timer;
        obj->normlizeSegment();
        timer.timeFromBegin();
    }
}

void MainWindowOcc::handleLeftPress(const double& _1, const double& _2)
{
    if (curmode_ == AppModeEnum::draw_line)
    {
        if (!line_drawer_)
        {
            line_drawer_ = new LineDrawer(gp_Pnt(_1, _2, 0.0));
            return;
        }
        line_drawer_->appendLine(gp_Pnt(_1, _2, 0.0));
    }
}

void MainWindowOcc::handleRightPress(QMouseEvent* event)
{
    QPointer<QMenu> rightMenu = getRightMenu();
    if (rightMenu->isEmpty())return;
    rightMenu->exec(cursor().pos());
}

void MainWindowOcc::handleMouseMove(const double& _1, const double& _2, const double& _3)
{
    if (curmode_ == AppModeEnum::draw_line)
    {
        if (line_drawer_)
        {
            line_drawer_->drawTempLine(gp_Pnt(_1, _2, 0.0));
        }
    }
    this->setStatusBar(_1, _2, _3);
}

void MainWindowOcc::setStatusBar(const double& _1, const double& _2, const double& _3)
{
    info_widget_->setMessage(_1, _2, _3);
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
    cadview_->fitAll();
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
    kdtree_->getTwoDRange(KRegion(KPt(ui->dsb_left->value(), ui->dsb_down->value()),
        KPt(ui->dsb_right->value(), ui->dsb_up->value())));
}

void MainWindowOcc::on_actionori_find2D_triggered()
{
    if (!kdtree_)kdtree_ = new KDTree(data_generator_);
    kdtree_->getTwoDRangeOri(KRegion(KPt(ui->dsb_left->value(), ui->dsb_down->value()),
        KPt(ui->dsb_right->value(), ui->dsb_up->value())));
}

void MainWindowOcc::on_pb_valueMax_pressed()
{
    ui->sb_col->setValue(300);
    ui->sb_row->setValue(300);
}
void MainWindowOcc::on_pb_valueMax_unsafe_pressed()
{
    ui->sb_col->setValue(1000);
    ui->sb_row->setValue(1000);
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

QMenu* MainWindowOcc::getRightMenu()
{
    QMenu* right_button_menu = new QMenu(this);
    right_button_menu->setStyleSheet(" ");
    switch (curmode_)
    {
    case KDebugger::MainWindowOcc::AppModeEnum::none:
        break;
    case KDebugger::MainWindowOcc::AppModeEnum::draw_line:
    {
        right_button_menu->addAction(tr("commit draw"), this,
            [&]() { execCmd(CmdEnum::commit_draw); });
        right_button_menu->addAction(tr("cancel draw"), this,
            [&]() { execCmd(CmdEnum::cancel_draw); });
        break;
    }
    default:
        break;
    }


    return right_button_menu;
}

void MainWindowOcc::execCmd(CmdEnum _cmd)
{
    switch (_cmd)
    {
    case KDebugger::MainWindowOcc::CmdEnum::commit_draw:
    {
        if (!line_drawer_)return;
        std::list<gp_Pnt> res;
        line_drawer_->commitDraw(res);
        PrePline* new_line = new PrePline(res);
        preline_vec_.push_back(new_line);

        new_line->drawPnts();
        curmode_ = AppModeEnum::none;
        delete line_drawer_;
        line_drawer_ = nullptr;
        break;
    }
    case KDebugger::MainWindowOcc::CmdEnum::cancel_draw:
    {
        if (!line_drawer_) return;
        line_drawer_->cancelDraw();
        curmode_ = AppModeEnum::none;
        delete line_drawer_;
        line_drawer_ = nullptr;
        break;
    }
    default:
        break;
    }

};
}