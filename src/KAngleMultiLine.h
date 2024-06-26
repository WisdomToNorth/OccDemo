﻿#pragma once

#include "KLine.h"
#include "KPnt.h"
#include <vector>
#include <list>
#include <deque>
#include <gp_Pnt.hxx>
#include "KBox.h"
#include <TopoDS_Edge.hxx>

namespace KDebugger
{

class KAngleMultiLine
{
public:
    enum class Dir
    {
        up,
        down,
        left,
        right
    };

    KAngleMultiLine(const KPt &p1, const KPt &p2);
    KAngleMultiLine(const gp_Pnt &p1, const gp_Pnt &p2);

    void cpuAvailable(double angle);
    bool checkColli(const KBox &box);
    gp_Pnt getRes() const;
    bool checkBetterRes(const KAngleMultiLine &previous_line);
    double getPreviousRes(const KAngleMultiLine &previous_line);
    size_t resSize()
    {
        return res_.size();
    }
    gp_Pnt p1_;
    gp_Pnt p2_;
    std::vector<TopoDS_Edge> getEdge();

private:
    std::list<gp_Pnt> drawAngledLineByTwoPts(gp_Pnt pA,
                                             gp_Pnt pB, const double &angle);

private:
    std::list<gp_Pnt> res_;
};

} // namespace KDebugger