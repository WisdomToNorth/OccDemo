#pragma once

#include "KPnt.h"

class gp_Pnt;
namespace KDebugger
{
struct KLine
{
public:
    KLine(const KPt &p1, const KPt &p2) :
        p1_(p1), p2_(p2)
    {}
    KLine(const gp_Pnt &p1, const gp_Pnt &p2);
    bool isCross(const KLine &rhs) const;

private:
    KPt p1_;
    KPt p2_;
};
} // namespace KDebugger
