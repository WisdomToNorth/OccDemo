#include "K_Pnt.h"
#include <iostream>



namespace KDebugger
{

void printPntVec(const std::vector<KPt>& pnts)
{
    int n = pnts.size();
    for (int i = 0; i < n; ++i)
    {
        std::cout << "{" << pnts[i].x << ", " << pnts[i].y << "}";
        if (!((i + 1) % 5))
        {
            std::cout << "\n";
        }
        else
            std::cout << '\t';
    }

}

}