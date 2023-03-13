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
    void print()const;
    void printY()const;
    void printX()const;
};
void printPntVec(const std::vector<KPt>& pnts);

struct PntsSorted2D
{
    PntsSorted2D(const std::vector<KPt>& pnts);
    PntsSorted2D() {};


    PntsSorted2D getSubPntsInRangeX(double x_s, double x_m);
    std::vector<PntsSorted2D> getSubPntsByX(double _x);
    std::vector<PntsSorted2D> getSubPntsByY(double _y);
    KPt getSubPntsByMidX(std::vector<PntsSorted2D>& res);
    KPt getSubPntsByMidY(std::vector<PntsSorted2D>& res);

    template<typename Iterator>
    PntsSorted2D getSubPntsInRangeXIt(Iterator it_s, Iterator it_m);
    template<typename Iterator>
    PntsSorted2D getSubPntsInRangeYIt(Iterator it_s, Iterator it_m);
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
    bool confirmValid();
    void print();
};


}