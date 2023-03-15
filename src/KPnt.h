#pragma once

#include <assert.h>
#include <vector>

namespace KDebugger
{
struct KBoxObj;
struct KPt
{
    KPt(double _x, double _y, KPt* parent = nullptr) :
        x(_x), y(_y), parent_(parent)
    {
    };
    KPt(const KPt& rhs, KPt* parent = nullptr) :
        x(rhs.x), y(rhs.y), parent_(parent)
    {
    };
    KPt(KPt& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        parent_ = rhs.parent_;
        //rhs.parent_ = nullptr;
    }
    KPt() :x(0), y(0), parent_(nullptr) {};
    bool isEqual(const KPt& rhs);
    bool operator==(const KPt& rhs);
    void print()const;
    void printY()const;
    void printX()const;

    double x;
    double y;
    KPt* parent_ = nullptr;
    KBoxObj* obj_ptr_ = nullptr;
    bool merged_ = false;
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