#include "DataGenerator.h"


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
#include "KLogger.h"
#include "global.h"
#include "DataObserver.h"
#include "public_function.h"

namespace KDebugger
{
void DataGenerator::addToObserverList(DataObserver* obs)
{
    obs_list_.emplace_back(obs);
}

void DataGenerator::notifyAll()
{
    for (const auto& obs : obs_list_)
    {
        obs->updateData();
    }
}
void DataGenerator::getPtData(std::vector<KPt>& buf)
{
    std::vector<KPt> newbuf;
    for (const auto& box : buf_)
    {
        newbuf.emplace_back(box.getPt());
    }
    buf.swap(newbuf);

}

void DataGenerator::generateData(std::vector<KBox>& buffer,
    int _row_size, int _col_size, double distance, int precision,
    double radius, bool same_radius)
{
    if (_col_size == 0)_col_size = _row_size;


    std::default_random_engine e;
    std::uniform_real_distribution<double> sizeu(0.8, 1.2);//尺寸随机范围
    std::uniform_real_distribution<double> v(0.6, 0.95);//位置随机范围
    e.seed(1);
    for (int i = 0; i < _col_size; ++i)
    {
        for (int j = 0; j < _row_size; ++j)
        {
            double loc_random = v(e);
            double stx = roundWith(i * distance * loc_random, precision);
            double sty = roundWith(j * distance * loc_random, precision);
            double x_size, y_size;
            x_size = radius * sizeu(e);
            if (same_radius)
            {
                y_size = x_size;
            }
            else
            {
                y_size = radius * sizeu(e);
            }

            KBox l_box(stx, sty, x_size, y_size);
            l_box.val_ = i * _row_size + j;
            buffer.emplace_back(l_box);
        }
    }
    ConsoleLog("Size: " + QString::number(_row_size * distance) + " * " +
        QString::number(_col_size * distance));
    notifyAll();
}

void DataGenerator::reGenerateData(int rowcnt, int colcnt, double dis, int precision,
    double radius, bool same_radius)
{
    if (buf_.size() > 0)
    {
        std::vector<KBox> newbuf;
        buf_.swap(newbuf);
    }
    ConsoleLog("generating...");
    K_Timer timer;

    generateData(buf_, rowcnt, colcnt, dis, precision, 1.0, same_radius);

    if (rowcnt * colcnt < 1000)
    {
        viewData();
    }
    ConsoleLog("generate data cost " + QString::number(timer.timeFromBegin(false))
        + " ms.");
    notifyAll();
}


void DataGenerator::viewData()
{
    viewer_->removeAll();
    for (auto& box : buf_)
    {
        box.show();
    }

    viewer_->fitAll();

    viewer_->update();
}
}