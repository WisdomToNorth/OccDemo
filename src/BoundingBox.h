#pragma once

#include "KPnt.h"
#include "KBox.h"
#include "KLine.h"

class gp_Pnt;
namespace KDebugger
{
class KBox;
struct KBoundingBox
{
public:
    KBoundingBox(const KPt& lb, const KPt& ru) :lb_(lb), ru_(ru) {}
    KBoundingBox(const gp_Pnt& pt);
    KBoundingBox(const KPt& pt) :lb_(pt), ru_(pt) {}
    KBoundingBox(const KBox& box);
    bool isOut(const KBoundingBox& rhs)const;
    void update(const KPt& lb);
    bool isCrossKLine(const KLine& line)const;
    bool ptInRegion(const KPt& pnt_)const;//[ )
    bool ptInRegionExpend(const KPt& pnt_)const;//[ )

public:
    KPt lb_;
    KPt ru_;
};

}