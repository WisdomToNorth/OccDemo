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
bool KPt::operator==(const KPt& rhs)
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

bool mycompx(const KPt& pt1, const KPt& pt2)
{
    return pt1.x < pt2.x;
}

KPt PntsSorted2D::getSubPntsByMidX(PntsSorted2D& p1, PntsSorted2D& p2)
{

    size_t cursize = this->pnts_xsorted_.size();
    if (cursize == 1)
    {
        p2 = *this;
        return this->getOnlyPnt();
    }

    size_t mid = cursize / 2;
    auto it = this->pnts_xsorted_.begin();
    std::advance(it, mid);
    double pivot = it->x;

    auto second_it = std::lower_bound(pnts_xsorted_.begin(),
        pnts_xsorted_.end(), *it, mycompx);


    p1.pnts_xsorted_.assign(pnts_xsorted_.begin(), second_it);
    p2.pnts_xsorted_.assign(second_it, pnts_xsorted_.end());

    auto it_y = std::stable_partition(pnts_ysorted_.begin(), pnts_ysorted_.end(),
        [&pivot](const KPt& pt)
        {
            return pt.x < pivot;
        });

    p1.pnts_ysorted_.assign(pnts_ysorted_.begin(), it_y);
    p2.pnts_ysorted_.assign(it_y, pnts_ysorted_.end());

    assert(p1.confirmValid() && p2.confirmValid());

    return *it;
}

bool mycompy(const KPt& pt1, const KPt& pt2)
{
    return pt1.y < pt2.y;
}

KPt PntsSorted2D::getSubPntsByMidY(PntsSorted2D& p1, PntsSorted2D& p2)
{
    size_t cursize = this->pnts_xsorted_.size();
    if (cursize == 1)
    {
        p2 = *this;
        return this->getOnlyPnt();
    }

    size_t mid = this->pnts_ysorted_.size() / 2;
    auto it = this->pnts_ysorted_.begin();
    std::advance(it, mid);
    double pivot = it->y;

    auto second_it = std::lower_bound(pnts_ysorted_.begin(),
        pnts_ysorted_.end(), *it, mycompy);

    p1.pnts_ysorted_.assign(pnts_ysorted_.begin(), second_it);
    p2.pnts_ysorted_.assign(second_it, pnts_ysorted_.end());

    auto it_y = std::stable_partition(pnts_xsorted_.begin(), pnts_xsorted_.end(),
        [&](const KPt& pt)
        {
            return pt.y < pivot;
        });

    p1.pnts_xsorted_.assign(pnts_xsorted_.begin(), it_y);
    p2.pnts_xsorted_.assign(it_y, pnts_xsorted_.end());
    assert(p1.confirmValid() && p2.confirmValid());

    return *it;
}

void PntsSorted2D::print()
{
    std::cout << "\npntsx:"; printPntVec(pnts_xsorted_);
    std::cout << "\npntsy:"; printPntVec(pnts_ysorted_);
}
}