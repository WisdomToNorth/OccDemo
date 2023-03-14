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
    bool isEqual(const KPt& rhs);
    bool operator<(const KPt& rhs)
    {
        return this->y < rhs.y;
    }

    bool operator==(const KPt& rhs);
    void print()const;
    void printY()const;
    void printX()const;
};

void printPntVec(const std::vector<KPt>& pnts);

struct PntsSorted2D
{
    PntsSorted2D(const std::vector<KPt>& pnts);
    PntsSorted2D() {};

    KPt getSubPntsByMidX(PntsSorted2D& p1, PntsSorted2D& p2);
    KPt getSubPntsByMidY(PntsSorted2D& p1, PntsSorted2D& p2);

    std::vector<KPt> pnts_xsorted_;
    std::vector<KPt> pnts_ysorted_;

    bool empty()const
    {
        assert(pnts_xsorted_.empty() == pnts_ysorted_.empty());
        return pnts_xsorted_.empty();
    }
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
    bool confirmValid();
    void print();
};


}