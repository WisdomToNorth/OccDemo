#pragma once

#include "K_Pnt.h"
#include "K_box.h"

class gp_Pnt;
namespace KDebugger
{
class KBox;
class KBoundingBox
{
public:
    KBoundingBox(const KPt& lb, const KPt& ru) :lb_(lb), ru_(ru) {}
    KBoundingBox(const gp_Pnt& pt);
    KBoundingBox(const KPt& pt) :lb_(pt), ru_(pt) {}
    KBoundingBox(const KBox& box);
    bool isOut(const KBoundingBox& rhs)const;
    void update(const KPt& lb);

private:
    KPt lb_;
    KPt ru_;


};

}