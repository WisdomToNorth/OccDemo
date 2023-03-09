#include "KPnt.h"
#include <iostream>



namespace KDebugger
{

bool KPt::operator <(const KPt& rhs)
{
    return this->x < rhs.x;
}

bool KPt::operator >(const KPt& rhs)
{
    return this->x < rhs.x;
}

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

}