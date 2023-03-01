#pragma once

#include "KLine.h"
#include "K_Pnt.h"
#include <vector>
#include <gp_Pnt.hxx>
#include "K_box.h"

namespace KDebugger
{

struct KAngleMultiLine
{
    KAngleMultiLine(const KPt& p1, const KPt& p2);
    KAngleMultiLine(const gp_Pnt& p1, const gp_Pnt& p2);

    void cpuAvailable(double angle);
    bool checkColli(const KBox& box);
    gp_Pnt getRes();
    size_t resSize() { return res_.size(); }
private:
    gp_Pnt p1_;
    gp_Pnt p2_;
    std::vector<gp_Pnt>res_;
    //std::vector<KLine>klines_;
    bool coli_1_ = false;
    bool coli_2_ = false;
};

}