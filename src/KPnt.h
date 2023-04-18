#pragma once

#include <assert.h>
#include <vector>

#include <gp_Pnt.hxx>

namespace KDebugger
{
class KBox;
struct KPt
{
    KPt(double _x, double _y, const KBox *parent = nullptr) :
        x(_x), y(_y), parent_(parent){};

    KPt(const gp_Pnt &pnt);
    KPt() :
        x(0), y(0), parent_(nullptr){};
    bool isEqual(const KPt &rhs);
    bool operator==(const KPt &rhs);
    void print() const;
    void printY() const;
    void printX() const;
    double distance(const KPt &rhs) const
    {
        return 0.0;
    } // todo
    double x;
    double y;

    const KBox *parent_;
    bool merged_ = false;
};

void printPntVec(const std::vector<KPt> &pnts);

struct PntsSorted2D
{
    PntsSorted2D(const std::vector<KPt> &pnts);
    PntsSorted2D(){};

    KPt getSubPntsByMidX_uneven(PntsSorted2D &p1, PntsSorted2D &p2);
    KPt getSubPntsByMidY_uneven(PntsSorted2D &p1, PntsSorted2D &p2);
    KPt getSubPntsByMidX(PntsSorted2D &p1, PntsSorted2D &p2);
    KPt getSubPntsByMidY(PntsSorted2D &p1, PntsSorted2D &p2);

    std::vector<KPt> pnts_xsorted_;
    std::vector<KPt> pnts_ysorted_;

    bool empty() const
    {
        assert(pnts_xsorted_.empty() == pnts_ysorted_.empty());
        return pnts_xsorted_.empty();
    }
    size_t size() const
    {
        assert(pnts_xsorted_.size() == pnts_ysorted_.size());
        return pnts_xsorted_.size();
    }

    KPt getOnlyPnt() const
    {
        assert(size() == 1);
        return pnts_xsorted_.front();
    }
    bool confirmValid();
    void print();
};

} // namespace KDebugger