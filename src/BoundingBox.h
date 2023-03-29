#pragma once

#include "KPnt.h"
#include "KBox.h"
#include "KLine.h"

class gp_Pnt;
namespace KDebugger
{

struct KBoundingBox
{
public:
    KBoundingBox(const KPt& p1, const KPt& p2);
    KBoundingBox(const gp_Pnt& pt);
    KBoundingBox(const KPt& pt) :lb_(pt), ru_(pt) {}
    KBoundingBox(const KBox& box);
    bool isOut(const KBoundingBox& rhs)const;
    void update(const KPt& lb);
    bool isCrossKLine(const KLine& line)const;
    bool ptInRegion(const KPt& pnt_)const;//[ )
    bool ptInRegionExpend(const KPt& pnt_)const;//[ )

    double left()const { return lb_.x; }
    double right()const { return ru_.x; }
    double top()const { return ru_.y; }
    double bottom()const { return lb_.y; }
    double dx()const { return std::abs(lb_.x - ru_.x); }
    double dy()const { return std::abs(lb_.y - ru_.y); }
public:
    KPt lb_;
    KPt ru_;
    KPt rb_;
    KPt lu_;

};

}