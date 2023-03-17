#include "MainWindowOcc.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>
#include <thread>
#include <algorithm>

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QStatusBar>
#include <QMenu>
#include <QPointer>
#include <QDesktopWidget>

#include "CadView.h"
#include "KTimer.h"
#include "CustomQlistWidget.h"
#include "KLogger.h"
#include "global.h"
#include "MultiUniset.h"
#include "DataGenerator.h"
#include "2d_search.h"
#include "KPnt.h"
#include "statusinfowidget.h"
#include "linedrawer.h"
#include "PreLine.h"

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


    cadview_ = new CadView(this);
    G_Context = cadview_->getContext();
    cadview_->moveInfoCb = std::bind(&MainWindowOcc::handleMouseMove,
        this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    cadview_->leftClickCb = std::bind(&MainWindowOcc::handleLeftPress,
        this, std::placeholders::_1, std::placeholders::_2);
    cadview_->rightClickCb = std::bind(&MainWindowOcc::handleRightPress,
        this, std::placeholders::_1);


    info_widget_ = new StatusInfoWidget(this);
    ui->gl_info->addWidget(info_widget_);


    setUpUI();
    data_generator_ = new DataGenerator(cadview_);
    ui->gridLayout_view->addWidget(cadview_);
    ConsoleLog("Hello!");
    on_pb_generate_pressed();
    on_actionview_triggered();
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
        KTimer timer;
        obj->reGenerate(context_info);
        timer.timeFromBegin();
    }
}

void MainWindowOcc::on_action_normline_triggered()
{
    for (auto& obj : preline_vec_)
    {
        KTimer timer;
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

    auto scwidth = width * 3 / 4;
    auto scheight = height * 3 / 4;
    this->setGeometry(width * 1 / 8, height / 8, scwidth, scheight);

    iwCustomQListWidget* console_widget = InitConsole(this, this, false);
    ui->gl_console->addWidget(console_widget);

    QList<int> console_size;
    console_size << 3500 << 1000;
    ui->splitter->setSizes(console_size);
    QList<int> console_size_vert;
    console_size_vert << 1000 << 3000;
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
void MainWindowOcc::on_actionuf_opt3_triggered()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    int def = 0;
    if (ui->sb_core->value() > 0)
    {
        def = ui->sb_core->value();
    }
    unionset_->optUnionSet(def);
}

void MainWindowOcc::on_actionbi_find1D_triggered()
{
    kd_find1D(ui->cb_debug->isChecked());
}

void MainWindowOcc::on_actionori_find1D_triggered()
{
    ori_find1D(ui->cb_debug->isChecked());
}

int MainWindowOcc::kd_find1D(bool _debug)
{
    if (!kdtree_)kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getOneDRange(ui->dsb_down->value(), ui->dsb_up->value(), _debug);
}
int MainWindowOcc::ori_find1D(bool _debug)
{
    if (!kdtree_)kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getOneDRangeOri(ui->dsb_down->value(),
        ui->dsb_up->value(), _debug);
}
int MainWindowOcc::kd_find2D(bool _debug)
{
    return 0;
}
int MainWindowOcc::ori_find2D(bool _debug)
{
    if (!kdtree_)kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getTwoDRangeOri(KRegion(KPt(ui->dsb_left->value(), ui->dsb_down->value()),
        KPt(ui->dsb_right->value(), ui->dsb_up->value())), _debug);
}

int MainWindowOcc::ran_find2D(bool _debug)
{
    if (!kdtree_)kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getTwoDRangeRangeTree(KRegion(KPt(ui->dsb_left->value(), ui->dsb_down->value()),
        KPt(ui->dsb_right->value(), ui->dsb_up->value())), _debug);
}
void MainWindowOcc::on_actionkd_find2D_triggered()
{
    kd_find2D(ui->cb_debug->isChecked());
}

void MainWindowOcc::on_actionori_find2D_triggered()
{
    ori_find2D(ui->cb_debug->isChecked());
}

void MainWindowOcc::on_actionran_find2D_triggered()
{
    ran_find2D(ui->cb_debug->isChecked());
}

void MainWindowOcc::on_pb_valueMax_pressed()
{
    ui->sb_col->setValue(300);
    ui->sb_row->setValue(300);
    on_pb_generate_pressed();
}
void MainWindowOcc::on_pb_valueMax_unsafe_pressed()
{
    ui->sb_col->setValue(1000);
    ui->sb_row->setValue(1000);
    on_pb_generate_pressed();
}

void MainWindowOcc::on_pb_Randvalue_pressed()
{
    int mi = 0;
    int ma = ui->sb_maxvalue->value();
    const std::vector<int>& nums = data_generator_->getIntNumbers(mi, ma, 4);
    ui->sb_col->setValue(nums[0]);
    ui->sb_row->setValue(nums[3]);
    ui->sb_precision->setValue(nums[1] % 3);
}

void MainWindowOcc::on_pb_RandRange_pressed()
{
    double mi = ui->dsb_r_min->value();
    double ma = ui->dsb_r_max->value();
    const std::vector<double>& nums = data_generator_->getFourNumber(mi, ma);
    ui->dsb_left->setValue(nums[0]);
    ui->dsb_down->setValue(nums[1]);
    ui->dsb_up->setValue(nums[2]);
    ui->dsb_right->setValue(nums[3]);
}

void MainWindowOcc::on_pb_generate_pressed()
{
    DataParameter param;
    param.colcnt = ui->sb_col->value();
    param.rowcnt = ui->sb_row->value();
    param.seed = ui->sb_seed->value();

    G_Random_Engine.seed(param.seed);

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

    data_generator_->reGenerateData(param, false);

    if (ui->sb_col->value() < 10 &&
        ui->sb_row->value() < 10)
        this->on_actionview_triggered();
}

void MainWindowOcc::on_pb_appenddata_pressed()
{

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

        new_line->drawRawPnts();
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

void MainWindowOcc::on_pb_Test1DFind_pressed()
{
    std::cout << "\n\n\n############ 1D Search Test Start ###########" << std::endl;
    KTimer timer;
    for (int j = 0; j < 10; ++j)
    {
        on_pb_Randvalue_pressed();
        on_pb_generate_pressed();
        for (int i = 0; i < 100; i++)
        {
            on_pb_RandRange_pressed();
            if (ori_find1D() != kd_find1D())
            {
                std::cout << "\n\n****** test failed:" << i << " ******" << std::endl;
                std::cout << "range: [" << ui->dsb_down->value() << ", "
                    << ui->dsb_up->value() << "): ";
                std::cout << "ori :" << ori_find1D() << std::endl;
                std::cout << "binfind :" << kd_find1D() << std::endl;
                std::cout << "ori :";
                ori_find1D(true);
                std::cout << "binfind :";
                kd_find1D(true);
                return;
            }
        }
    }
    std::cout << "test 10*100 times in " << timer.timeFromBegin(false) << " ms";
    std::cout << "\n############ 1D Search Test Done ###########\n";

}
void MainWindowOcc::on_pb_Simple1DFind_pressed()
{
    std::cout << "\n\n\n############ 1D Search Test Start ###########" << std::endl;
    KTimer timer;
    for (int i = 0; i < 100; i++)
    {
        on_pb_RandRange_pressed();
        if (ori_find1D() != kd_find1D())
        {
            std::cout << "\n\n****** test failed:" << i << " ******" << std::endl;
            std::cout << "range: [" << ui->dsb_down->value() << ", "
                << ui->dsb_up->value() << "): ";
            std::cout << "ori :" << ori_find1D() << std::endl;
            std::cout << "binfind :" << kd_find1D() << std::endl;
            std::cout << "ori :";
            ori_find1D(true);
            std::cout << "binfind :";
            kd_find1D(true);
            return;
        }
    }
    std::cout << "test 100 times in " << timer.timeFromBegin(false) << " ms";
    std::cout << "\n############ 1D Search Test Done ###########\n";
}

void MainWindowOcc::on_pb_Simple2DFind_pressed()
{
    std::cout << "\n\n\n############ 2D Search Test Start #############" << std::endl;
    KTimer timer;
    /*bool succ = true;
    for (int i = 0; i < 1000; i++)
    {
        on_pb_RandRange_pressed();
        ori_find2D();
    }
    std::cout << "ori: " << std::endl;
    timer.timeFromBegin();
    for (int i = 0; i < 1000; i++)
    {
        on_pb_RandRange_pressed();
        ran_find2D();
    }
    std::cout << "\nrange tree: " << std::endl;
    timer.timeFromLastSee();
    std::cout << "\n############ 2D Search Test Done #############" << std::endl;
    std::cout << "test result: " << succ << std::endl;*/

    for (int i = 0; i < 100; i++)
    {
        on_pb_RandRange_pressed();
        if (ori_find2D() != ran_find2D())
        {
            std::cout << "\n\n****** test failed:" << i << " ******" << std::endl;
            std::cout << "left to right: [" << ui->dsb_left->value() << ", "
                << ui->dsb_right->value() << ")" << std::endl;
            std::cout << "down to up: [" << ui->dsb_down->value() << ", "
                << ui->dsb_up->value() << ")" << std::endl;

            std::cout << "number of ori :" << ori_find2D() << std::endl;
            std::cout << "number of range tree :" << ran_find2D() << std::endl;
            std::cout << "ori :";
            ori_find2D(true);
            std::cout << "\nrange tree :";
            ran_find2D(true);
            return;
        }
    }

}

void MainWindowOcc::on_pb_Test2DFind_pressed()
{
    std::cout << "\n\n\n############ 2D Search Test Start #############" << std::endl;
    KTimer timer;
    for (int j = 0; j < 10; ++j)
    {
        on_pb_Randvalue_pressed();
        on_pb_generate_pressed();
        for (int i = 0; i < 100; i++)
        {
            on_pb_RandRange_pressed();
            if (ori_find2D() != ran_find2D())
            {
                std::cout << "\n\n****** test failed:" << i << " ******" << std::endl;
                std::cout << "left to right: [" << ui->dsb_left->value() << ", "
                    << ui->dsb_right->value() << ")" << std::endl;
                std::cout << "down to up: [" << ui->dsb_down->value() << ", "
                    << ui->dsb_up->value() << ")" << std::endl;

                std::cout << "number of ori :" << ori_find2D() << std::endl;
                std::cout << "number of range tree :" << ran_find2D() << std::endl;
                std::cout << "ori :";
                ori_find2D(true);
                std::cout << "\nrange tree :";
                ran_find2D(true);
                return;
            }
        }
    }
    std::cout << "test 10*100 times in " << timer.timeFromBegin(false) << " ms";
    std::cout << "\n############ 2D Search Test Done #############" << std::endl;
}

}