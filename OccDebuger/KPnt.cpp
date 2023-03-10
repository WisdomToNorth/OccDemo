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
}