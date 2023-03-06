#pragma once

#include <assert.h>
#include <vector>

namespace KDebugger
{
struct KPt
{
    double x;
    double y;
    KPt(double _x, double _y) :x(_x), y(_y) {};
    KPt() :x(0), y(0) {};
    void print()const;
    void printX()const;
};
void printPntVec(const std::vector<KPt>& pnts);


struct PntsSorted2D
{
    std::vector<KPt> pnts_xsorted_;
    std::vector<KPt> pnts_ysorted_;

    size_t size()const
    {
        assert(pnts_xsorted_.size() == pnts_ysorted_.size());
        return pnts_xsorted_.size();
    }

    KPt getOnlyPnt()const
    {
        assert(size() == 1);
        return pnts_xsorted_.front();
    }
};

}