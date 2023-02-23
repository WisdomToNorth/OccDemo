#include "K_Pnt.h"
#include <iostream>



namespace KDebugger
{
void KPt::print()const
{
    std::cout << "\n{" << x << ", " << y << "}";
}



}