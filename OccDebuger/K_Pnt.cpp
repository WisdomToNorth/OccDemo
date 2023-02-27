#include "K_Pnt.h"
#include <iostream>



namespace KDebugger
{
void KPt::print()const
{
    std::cout << "{" << x << ", " << y << "}";
}
void KPt::printX()const
{
    std::cout << "{" << x << "}";
}

void printPntVec(const std::vector<KPt>& pnts)
{
    int n = pnts.size();
    for (int i = 0; i < n; ++i)
    {
        pnts[i].print();
        if (!((i + 1) % 5))
        {
            std::cout << "\n";
        }
        else
            std::cout << '\t';
    }

}

bool ptInRegion(const KPt& pnt_, const KRegion& region)//[ )
{
    return (pnt_.x >= region.left_bottom_.x) &&
        (pnt_.x < region.right_up_.x) &&
        (pnt_.y >= region.left_bottom_.y) &&
        (pnt_.y < region.right_up_.y);
}
}