#include "MainWindowOcc.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include <QCheckBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QScreen>
#include <QSpinBox>
#include <QStatusBar>

#include "2d_search.h"
#include "CadView.h"
#include "CustomQlistWidget.h"
#include "DataGenerator.h"
#include "KLogger.h"
#include "KPnt.h"
#include "KTimer.h"
#include "MultiUniset.h"
#include "PreLine.h"
#include "global.h"
#include "linedrawer.h"
#include "statusinfowidget.h"

#include "ui_MainWindowOcc.h"

namespace KDebugger
{
MainWindowOcc::MainWindowOcc(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindowOccClass()), unionset_(nullptr), kdtree_(nullptr)

{
    ui->setupUi(this);

    this->setStyleSheet(G_GetUiStyleSheet(getUIConfig()));
    auto screenRect = QGuiApplication::screens();
    auto width = screenRect[0]->geometry().width();
    auto height = screenRect[0]->geometry().height();

    auto scwidth = width * 3 / 4;
    auto scheight = height * 3 / 4;
    this->setGeometry(width * 1 / 8, height / 8, scwidth, scheight);

    cadview_ = new CadView(this), G_Context = cadview_->getContext();

    // ui->widget_param->setMaximumWidth(scwidth / 3);
    ui->gridLayout_view->addWidget(cadview_);

    QList<int> console_size_vert;
    console_size_vert << 1000 << 3000;
    ui->splitter_main->setSizes(console_size_vert);
    // ui->splitter_main->setChildrenCollapsible(false);

    QList<int> console_size;
    console_size << 3500 << 1000;
    ui->splitter_view->setSizes(console_size);

    info_widget_ = new StatusInfoWidget(this);
    ui->gl_info->addWidget(info_widget_);
    iwCustomQListWidget *console_widget = InitConsole(this, this, false);
    ui->gl_console->addWidget(console_widget);
    ui->widget_info->hide();

    cadview_->moveInfoCb = std::bind(&MainWindowOcc::handleMouseMove, this, std::placeholders::_1,
                                     std::placeholders::_2, std::placeholders::_3);
    cadview_->leftClickCb = std::bind(&MainWindowOcc::handleLeftPress, this, std::placeholders::_1,
                                      std::placeholders::_2);
    cadview_->rightClickCb =
        std::bind(&MainWindowOcc::handleRightPress, this, std::placeholders::_1);

    data_generator_ = new DataGenerator(cadview_);

    KLog("Hello!");
    on_pb_generate_pressed();

    on_actionview_triggered();
}

MainWindowOcc::~MainWindowOcc()
{
    if (data_generator_)
        delete data_generator_;
    if (unionset_)
        delete unionset_;

    delete ui;
}

void MainWindowOcc::on_action_showLog_triggered()
{
    if (ui->widget_info->isVisible())
    {
        ui->widget_info->setVisible(false);
    }
    else
        ui->widget_info->show();
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
    const std::vector<KBox> &context_info = data_generator_->getData();
    for (auto &obj : preline_vec_)
    {
        KTimer timer;
        obj->reGenerate(context_info);
        timer.timeFromBegin();
    }
}

void MainWindowOcc::on_action_normline_triggered()
{
    for (auto &obj : preline_vec_)
    {
        KTimer timer;
        obj->normlizeSegment();
        timer.timeFromBegin();
    }
}

void MainWindowOcc::handleLeftPress(const double &_1, const double &_2)
{
    if (curmode_ == AppModeEnum::draw_line)
    {
        if (!line_drawer_)
        {
            line_drawer_ = new LineDrawer(gp_Pnt(_1, _2, 0.0));
            return;
        }
        double angle = ui->cb_arc45->isChecked() ? 45.0 : 0.0;
        bool toggle = ui->cb_toggle->isChecked() ? true : false;
        line_drawer_->appendLine(gp_Pnt(_1, _2, 0.0), angle, toggle);
    }
}

void MainWindowOcc::handleRightPress(QMouseEvent *event)
{
    QPointer<QMenu> rightMenu = getRightMenu();
    if (rightMenu->isEmpty())
        return;
    rightMenu->exec(cursor().pos());
}

void MainWindowOcc::handleMouseMove(const double &_1, const double &_2, const double &_3)
{
    this->setStatusBar(_1, _2, _3);
    processing_ = false;

    if (curmode_ == AppModeEnum::draw_line)
    {
        if (line_drawer_)
        {
            double angle = ui->cb_arc45->isChecked() ? 45.0 : 0.0;
            bool toggle = ui->cb_toggle->isChecked() ? true : false;
            line_drawer_->drawTempLine(gp_Pnt(_1, _2, 0.0), angle, toggle);
        }
    }
    else if (curmode_ == AppModeEnum::caculate)
    {
        KTimer timer;
        int res = 0;

        std::this_thread::sleep_for(std::chrono::microseconds(100));
        if (!lock_.try_lock())
        {
            // std::this_thread::sleep_for(std::chrono::microseconds(1));
            // QApplication::processEvents();
            return;
        }

        this->processing_ = true;
        MultiUniset solver(data_generator_);
        cadview_->setUserCursor(CadView::CursorType::wait);
        std::thread t([&] { solver.oneCoreUnionSet(res); });
        t.detach();
        while (solver.done_ == false)
        {
            QApplication::processEvents();
            if (!processing_)
            {
                solver.stop_ = true;
                // lock_.unlock();
                // return;
            }
        }
        if (processing_)
        {
            std::cout << "\nRES: " << res << std::endl;
            timer.timeFromBegin("Opt1: ");
        }
        cadview_->setUserCursor(CadView::CursorType::def);
        lock_.unlock();
    }
}

void MainWindowOcc::setStatusBar(const double &_1, const double &_2, const double &_3)
{
    if (!info_widget_)
        return;
    info_widget_->setMessage(_1, _2, _3);
}

void MainWindowOcc::setUpUI()
{
}

void MainWindowOcc::on_actionFitAll_triggered()
{
    cadview_->updateView(CadView::KUpdate::Fitall);
}
void MainWindowOcc::on_actionclear_triggered()
{
    std::cout << "\033c";
}
void MainWindowOcc::on_actionview_triggered()
{
    if (!data_generator_->viewData())
    {
        KLog("View data is too large, return.");
    }
}

// unionfind
void MainWindowOcc::on_act_unionfind_ori_triggered()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    unionset_->badWay();
}
// unionfind
void MainWindowOcc::on_act_unionfind_opt1_triggered()
{
    curmode_ = curmode_ == AppModeEnum::none ? AppModeEnum::caculate : AppModeEnum::none;
    std::cout << "Set Caculate: " << (curmode_ == AppModeEnum::caculate) << std::endl;
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
}

// unionfind
void MainWindowOcc::on_act_unionfind_opt2_triggered()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    int def = 0;
    if (ui->sb_core->value() > 0)
    {
        def = ui->sb_core->value();
    }
    KTimer timer;
    // res = this->unionset_->oneCoreUnionSet();
    std::cout << "res: " << unionset_->multiCoreUnionSet(def, true) << std::endl;

    std::cout << "opt2: ";
    timer.timeFromBegin("");
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
    KTimer timer;
    std::cout << "res: " << unionset_->optUnionSet(true) << std::endl; // todo:doesn't multi now
    std::cout << "opt3: ";
    timer.timeFromBegin("");
}

void MainWindowOcc::on_pb_TestUnionfind_pressed()
{
    if (!unionset_)
        unionset_ = new MultiUniset(data_generator_);
    int def = 0;
    if (ui->sb_core->value() > 0)
    {
        def = ui->sb_core->value();
    }
    for (int i = 0; i < 10; ++i)
    {
        on_pb_Randvalue_pressed();
        on_pb_generate_pressed();

        int ori;
        unionset_->oneCoreUnionSet(ori);
        int opt = unionset_->optUnionSet(def);
        if (ori == opt)
        {
            std::cout << "res: " << ori << std::endl;
        }
        else
        {
            std::cout << "#### test " << i << " ####\n";
            std::cout << "data size: " << ui->sb_row->value() << " * " << ui->sb_col->value()
                      << std::endl;
            std::cout << "ori: " << ori << std::endl;
            std::cout << "opt: " << opt << std::endl;
            std::cout << "test failed!";
            return;
        }
    }
    std::cout << "test " << 10 << " successful!\n";
    KTimer timer;
    for (int i = 0; i < 50; ++i)
    {
        on_pb_Randvalue_pressed();
        on_pb_generate_pressed();
        int dummy;
        unionset_->oneCoreUnionSet(dummy);
    }
    std::cout << "ori time: ";
    timer.timeFromBegin("");
    for (int i = 0; i < 50; ++i)
    {
        on_pb_Randvalue_pressed();
        on_pb_generate_pressed();
        unionset_->multiCoreUnionSet(def);
    }
    std::cout << "opt time: ";
    timer.timeFromLastSee("");
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
    if (!kdtree_)
        kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getOneDRange(ui->dsb_down->value(), ui->dsb_up->value(), _debug);
}
int MainWindowOcc::ori_find1D(bool _debug)
{
    if (!kdtree_)
        kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getOneDRangeOri(ui->dsb_down->value(), ui->dsb_up->value(), _debug);
}
int MainWindowOcc::kd_find2D(bool _debug)
{
    return 0;
}
int MainWindowOcc::ori_find2D(bool _debug)
{
    if (!kdtree_)
        kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getTwoDRangeOri(KRegion(KPt(ui->dsb_left->value(), ui->dsb_down->value()),
                                            KPt(ui->dsb_right->value(), ui->dsb_up->value())),
                                    _debug);
}

int MainWindowOcc::ran_find2D(bool _debug)
{
    if (!kdtree_)
        kdtree_ = new TwoDSearch(data_generator_);
    return kdtree_->getTwoDRangeRangeTree(KRegion(KPt(ui->dsb_left->value(), ui->dsb_down->value()),
                                                  KPt(ui->dsb_right->value(), ui->dsb_up->value())),
                                          _debug);
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
    const std::vector<int> &nums = data_generator_->getIntNumbers(mi, ma, 4);
    ui->sb_col->setValue(nums[0]);
    ui->sb_row->setValue(nums[3]);
    ui->sb_precision->setValue(nums[1] % 3);
}

void MainWindowOcc::on_pb_RandRange_pressed()
{
    double mi = ui->dsb_r_min->value();
    double ma = ui->dsb_r_max->value();
    const std::vector<double> &nums = data_generator_->getFourNumber(mi, ma);
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

    if (ui->sb_col->value() < 10 && ui->sb_row->value() < 10)
        this->on_actionview_triggered();
}

void MainWindowOcc::on_pb_appenddata_pressed()
{
}

QMenu *MainWindowOcc::getRightMenu()
{
    QMenu *right_button_menu = new QMenu(this);
    right_button_menu->setStyleSheet(" ");
    switch (curmode_)
    {
    case KDebugger::MainWindowOcc::AppModeEnum::none: break;
    case KDebugger::MainWindowOcc::AppModeEnum::draw_line:
    {
        right_button_menu->addAction(tr("commit draw"), this,
                                     [&]() { execCmd(CmdEnum::commit_draw); });
        right_button_menu->addAction(tr("cancel draw"), this,
                                     [&]() { execCmd(CmdEnum::cancel_draw); });
        break;
    }
    default: break;
    }

    return right_button_menu;
}

void MainWindowOcc::execCmd(CmdEnum _cmd)
{
    switch (_cmd)
    {
    case KDebugger::MainWindowOcc::CmdEnum::commit_draw:
    {
        if (!line_drawer_)
            return;
        std::list<gp_Pnt> res;
        line_drawer_->commitDraw(res);
        PrePline *new_line = new PrePline(res);
        preline_vec_.push_back(new_line);

        new_line->drawRawPnts();
        curmode_ = AppModeEnum::none;
        delete line_drawer_;
        line_drawer_ = nullptr;
        break;
    }
    case KDebugger::MainWindowOcc::CmdEnum::cancel_draw:
    {
        if (!line_drawer_)
            return;
        line_drawer_->cancelDraw();
        curmode_ = AppModeEnum::none;
        delete line_drawer_;
        line_drawer_ = nullptr;
        break;
    }
    default: break;
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
                std::cout << "range: [" << ui->dsb_down->value() << ", " << ui->dsb_up->value()
                          << "): ";
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
    std::cout << "test 10*100 times in ";
    timer.timeFromBegin("");
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
            std::cout << "range: [" << ui->dsb_down->value() << ", " << ui->dsb_up->value()
                      << "): ";
            std::cout << "ori :" << ori_find1D() << std::endl;
            std::cout << "binfind :" << kd_find1D() << std::endl;
            std::cout << "ori :";
            ori_find1D(true);
            std::cout << "binfind :";
            kd_find1D(true);
            return;
        }
    }
    std::cout << "test 100 times in ";
    timer.timeFromBegin("");
    std::cout << "\n############ 1D Search Test Done ###########\n";
}

void MainWindowOcc::on_pb_Simple2DFind_pressed()
{
    std::cout << "\n\n\n############ 2D Search Test Start #############" << std::endl;
    KTimer timer;
    ///////////// pre confirm /////////////
    ran_find2D();
    std::cout << "\nTime of build range tree: " << std::endl;
    timer.timeFromLastSee();
    for (int i = 0; i < 50; i++)
    {
        on_pb_RandRange_pressed();
        if (ori_find2D() != ran_find2D())
        {
            std::cout << "result mismatching, PLEASE CHECK";
            return;
        }
    }
    std::cout << "pre confirm succeed!\n";
    // test//
    ////////
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
                std::cout << "down to up: [" << ui->dsb_down->value() << ", " << ui->dsb_up->value()
                          << ")" << std::endl;

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
    std::cout << "test 10*100 times in ";
    timer.timeFromBegin("");
    std::cout << "\n############ 2D Search Test Successful #############" << std::endl;
}
} // namespace KDebugger