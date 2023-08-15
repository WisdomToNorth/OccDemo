#include "DataGenerator.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include <QMessageBox>
#include <qdebug.h>
#include <qguiapplication.h>
#include <qscreen.h>

#include "CadView.h"
#include "CustomQlistWidget.h"
#include "DataObserver.h"
#include "KLogger.h"
#include "KTimer.h"
#include "global.h"
#include "public_function.h"

namespace KDebugger
{
void DataGenerator::addToObserverList(DataObserver *obs)
{
    obs_list_.emplace_back(obs);
}

void DataGenerator::notifyAll()
{
    for (const auto &obs : obs_list_)
    {
        obs->updateData();
    }
}

void DataGenerator::getPtData(std::vector<KPt> &buf)
{
    std::vector<KPt> newbuf;
    for (const auto &box : buf_)
    {
        newbuf.emplace_back(box.getPt());
    }
    buf.swap(newbuf);
}

std::vector<int> DataGenerator::getIntNumbers(int _min, int _max, int count)
{
    std::vector<int> res;
    std::uniform_int_distribution<int> size_rand_gen(_min, _max); // 尺寸随机范围

    while (count--)
    {
        res.push_back(size_rand_gen(G_Random_Engine));
    }

    std::sort(res.begin(), res.end());
    return res;
}

std::vector<double> DataGenerator::getFourNumber(double _min, double _max)
{
    std::vector<double> res;
    std::uniform_real_distribution<double> size_rand_gen(_min, _max); // 尺寸随机范围

    res.push_back(size_rand_gen(G_Random_Engine));
    res.push_back(size_rand_gen(G_Random_Engine));
    res.push_back(size_rand_gen(G_Random_Engine));
    res.push_back(size_rand_gen(G_Random_Engine));
    std::sort(res.begin(), res.end());
    return res;
}
// void DataGenerator::generateData(std::vector<KBox>& buffer,
//     int _row_size, int _col_size, double distance, int precision,
//     double radius, bool same_radius)
void DataGenerator::generateData(std::vector<KBox> &buffer, const DataParameter &param)
{
    if (!checkParam(param))
        return;

    std::uniform_real_distribution<double> size_rand_gen(
        param.defaultpar.rand_size_min, param.defaultpar.rand_size_max); // 尺寸随机范围
    std::uniform_real_distribution<double> loc_rand_gen(
        param.defaultpar.rand_loc_min, param.defaultpar.rand_loc_max);   // 位置随机范围
    std::uniform_int_distribution<int> type_rand_gen(0, 5);

    for (int i = 0; i < param.colcnt; ++i)
    {
        for (int j = 0; j < param.rowcnt; ++j)
        {
            double loc_random = loc_rand_gen(G_Random_Engine);
            double stx = LeetCode::roundWith(i * param.dis * loc_random, param.precision);
            double sty = LeetCode::roundWith(j * param.dis * loc_random, param.precision);
            double x_size, y_size;

            x_size = param.defaultpar.base_size * size_rand_gen(G_Random_Engine);
            if (param.w_h_same)
            {
                y_size = x_size;
            }
            else // 如果size不设置随机，则xy的比例也为1:1（用同一个随机数生成器）
            {
                y_size = param.defaultpar.base_size * size_rand_gen(G_Random_Engine);
            }

            KBox l_box(stx, sty, x_size, y_size, type_rand_gen(G_Random_Engine));
            l_box.setVal(i * param.rowcnt + j);
            buffer.emplace_back(l_box);
        }
    }
    KLog("Crose range: " + QString::number(param.rowcnt * param.dis) + " * "
         + QString::number(param.colcnt * param.dis));
    notifyAll();
}
//
// void DataGenerator::reGenerateData(int rowcnt, int colcnt, double dis, int precision,
//    double radius, bool same_radius)
void DataGenerator::reGenerateData(const DataParameter &param, bool _view)
{
    if (buf_.size() > 0)
    {
        std::vector<KBox> newbuf;
        buf_.swap(newbuf);
    }
    KLog("generating...");
    KTimer timer;

    generateData(buf_, param);

    if (_view)
    {
        viewData();
    }
    KLog("generate " + QString::number(param.colcnt * param.rowcnt) + " data cost "
         + QString::number(timer.timeFromBegin("")) + " ms.");
    notifyAll();
}

bool DataGenerator::viewData()
{
    if (buf_.size() > 100)
    {
        return false;
    }
    cadview_->removeAll();

    for (auto &box : buf_)
    {
        box.show();
    }

    cadview_->updateView(CadView::KUpdate::Fitall);

    cadview_->update();
    return true;
}
} // namespace KDebugger