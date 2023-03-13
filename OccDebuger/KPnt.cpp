#include "KPnt.h"
#include <iostream>

#include "mathUtils.h"
#include "multisort.h"

namespace KDebugger
{

void KPt::print()const
{
    std::cout << "{" << x << ", " << y << "}";
}
void KPt::printY()const
{
    std::cout << "{" << y << "}";
}
void KPt::printX()const
{
    std::cout << "{" << x << "}";
}
bool KPt::isEqual(const KPt& rhs)
{
    return OccTools::fEqual(x, rhs.x) && OccTools::fEqual(y, rhs.y);
}

void printPntVec(const std::vector<KPt>& pnts)
{
    int n = pnts.size();
    for (int i = 0; i < n; ++i)
    {
        pnts[i].print();
        if (!((i + 1) % 5))
        {
            std::cout << std::endl;
        }
        else
            std::cout << '\t';
    }

}
bool PntsSorted2D::confirmValid()
{
    if (pnts_xsorted_.empty() && pnts_ysorted_.empty())return true;

    std::vector<KPt> p1 = pnts_xsorted_;
    std::vector<KPt> p2 = pnts_ysorted_;
    Sort_XL(p1.begin(), p1.end());
    Sort_XL(p2.begin(), p2.end());
    for (int i = 0; i < p1.size(); ++i)
    {
        if (!p1[i].isEqual(p2[i]))return false;
    }
    return true;
}

PntsSorted2D::PntsSorted2D(const std::vector<KPt>& pnts)
{
    pnts_xsorted_ = pnts;
    pnts_ysorted_ = pnts;

    Sort_XS(pnts_xsorted_.begin(), pnts_xsorted_.end());
    Sort_YS(pnts_ysorted_.begin(), pnts_ysorted_.end());
}

PntsSorted2D PntsSorted2D::getSubPntsInRangeX(double x_s, double x_m)
{

    size_t cnt = this->size();
    auto it = std::find_if(pnts_xsorted_.begin(), pnts_xsorted_.end(), [&](KPt& pt)
        {
            return OccTools::fEqual(pt.x, x_s);
        });
    auto it_er = std::find_if(pnts_xsorted_.rbegin(), pnts_xsorted_.rend(), [&](KPt& pt)
        {
            return OccTools::fEqual(pt.x, x_m);
        });

    auto it_e = pnts_xsorted_.begin();
    std::advance(it_e, cnt - std::distance(pnts_xsorted_.rbegin(), it_er));

    return getSubPntsInRangeXIt(it, it_e);
}

std::vector<PntsSorted2D> PntsSorted2D::getSubPntsByX(double _x)
{
    std::vector<PntsSorted2D> res;
    size_t cnt = this->size();
    auto it = std::find_if(pnts_xsorted_.begin(), pnts_xsorted_.end(), [&](KPt& pt)
        {
            return OccTools::fEqual(pt.x, _x) || pt.x > _x;;
        });

    PntsSorted2D res1;
    PntsSorted2D res2;
    res1.pnts_xsorted_.assign(pnts_xsorted_.begin(), it);
    res2.pnts_xsorted_.assign(it, pnts_xsorted_.end());

    for (size_t i = 0; i < cnt; ++i)
    {
        if (pnts_ysorted_[i].x < _x)
            res1.pnts_ysorted_.emplace_back(pnts_ysorted_[i]);
        else res2.pnts_ysorted_.emplace_back(pnts_ysorted_[i]);
    }

    assert(res1.confirmValid() && res2.confirmValid());
    res.emplace_back(res1);
    res.emplace_back(res2);

    return res;
}
std::vector<PntsSorted2D> PntsSorted2D::getSubPntsByY(double _y)
{
    std::vector<PntsSorted2D> res;
    size_t cnt = this->size();
    auto it = std::find_if(pnts_ysorted_.begin(), pnts_ysorted_.end(), [&](KPt& pt)
        {
            return OccTools::fEqual(pt.y, _y) || pt.y > _y;
        });

    PntsSorted2D res1;
    PntsSorted2D res2;
    res1.pnts_ysorted_.assign(pnts_ysorted_.begin(), it);
    res2.pnts_ysorted_.assign(it, pnts_ysorted_.end());

    for (size_t i = 0; i < cnt; ++i)
    {
        if (pnts_xsorted_[i].y < _y)
            res1.pnts_xsorted_.emplace_back(pnts_xsorted_[i]);
        else res2.pnts_xsorted_.emplace_back(pnts_xsorted_[i]);
    }

    assert(res1.confirmValid() && res2.confirmValid());

    res.emplace_back(res1);
    res.emplace_back(res2);

    return res;
}

KPt PntsSorted2D::getSubPntsByMidX(std::vector<PntsSorted2D>& res)
{
    KPt pt;
    return pt;
}

KPt PntsSorted2D::getSubPntsByMidY(std::vector<PntsSorted2D>& res)
{
    KPt pt;
    return pt;
}
template<typename Iterator>
PntsSorted2D PntsSorted2D::getSubPntsInRangeXIt(Iterator it_s, Iterator it_m)
{
    PntsSorted2D res;
    res.pnts_xsorted_.assign(it_s, it_m);
    double x_s = it_s->x;
    double x_m = it_s->x;
    size_t n = this->pnts_xsorted_.size();
    for (size_t i = 0; i < n; ++i)
    {
        if (pnts_ysorted_[i].x >= x_s
            && pnts_ysorted_[i].x < x_m)
            res.pnts_ysorted_.emplace_back(pnts_ysorted_[i]);
    }
    return res;
}

template<typename Iterator>
PntsSorted2D PntsSorted2D::getSubPntsInRangeYIt(Iterator it_s, Iterator it_m)
{
    PntsSorted2D res;
    res.pnts_xsorted_.assign(it_s, it_m);
    double y_s = it_s->y;
    double y_m = it_s->y;
    size_t n = this->pnts_xsorted_.size();
    for (size_t i = 0; i < n; ++i)
    {
        if (pnts_xsorted_[i].y >= y_s
            && pnts_xsorted_[i].y < y_m)
            res.pnts_xsorted_.emplace_back(pnts_xsorted_[i]);
    }
    return res;
}

void PntsSorted2D::print()
{
    std::cout << "\npntsx:"; printPntVec(pnts_xsorted_);
    std::cout << "\npntsy:"; printPntVec(pnts_ysorted_);
}
}