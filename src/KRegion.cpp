#include "KRegion.h"

namespace KDebugger
{

ERegionCrossState regionCheck(const KRegion &regionl,
                              const KRegion &regionr)
{
    if (regionl.isOut(regionr))
        return ERegionCrossState::out;
    else if (regionr.ptInRegion(regionl.lb_) && regionr.ptInRegion(regionl.ru_))
        return ERegionCrossState::in;
    else
        return ERegionCrossState::part;
}

bool ptInRegion(const KPt &pnt_, const KRegion &region) //[ )
{
    return (pnt_.x >= region.lb_.x) && (pnt_.x < region.ru_.x) && (pnt_.y >= region.lb_.y) && (pnt_.y < region.ru_.y);
}

} // namespace KDebugger