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
#include "RobotLogger.h"
#include "global.h"
#include "DataObserver.h"

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

void DataGenerator::generateTestData(std::vector<KBox>& buffer,
    int _row_size, int _col_size, double distance)
{
    if (_col_size == 0)_col_size = _row_size;


    std::default_random_engine e;
    std::uniform_real_distribution<double> sizeu(0.8, 1);
    std::uniform_real_distribution<double> v(0.6, 0.95);
    e.seed(1);
    for (int i = 0; i < _col_size; ++i)
    {
        for (int j = 0; j < _row_size; ++j)
        {
            double loc_random = v(e);
            double stx = i * distance * loc_random;
            double sty = j * distance * loc_random;
            double x_size = sizeu(e);
            double y_size = x_size;
            KBox l_box(stx, sty, x_size, y_size);
            buffer.emplace_back(l_box);
        }
    }
    ConsoleLog("Size: " + QString::number(_row_size * distance) + " * " +
        QString::number(_col_size * distance));
    notifyAll();
}

void DataGenerator::reGenerateData(int rowcnt, int colcnt, double dis)
{
    if (buf_.size() > 0)
    {
        std::vector<KBox> newbuf;
        buf_.swap(newbuf);
    }
    ConsoleLog("generating...");
    K_Timer timer;

    generateTestData(buf_, rowcnt, colcnt, dis);

    if (rowcnt * colcnt < 1000)
    {
        viewData();
    }
    ConsoleLog("generate data cost " + QString::number(timer.timeFromBegin(false)) + " ms.");
    notifyAll();
}


void DataGenerator::viewData()
{
    viewer_->removeAll();
    std::vector<Handle(AIS_Shape)> vecset;
    std::vector<Handle(AIS_TextLabel)> labs;
    KBox::generateViewData(buf_, vecset, labs);

    viewer_->drawTestData(vecset);
    viewer_->drawTestLabelData(labs);
    viewer_->fitAll();

    viewer_->update();
}
}