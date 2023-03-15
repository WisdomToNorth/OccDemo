#pragma once

#include "BoundingBox.h"


namespace KDebugger
{
typedef KBoundingBox KRegion;

enum class ERegionCrossState
{
    out, part, in
};

//note: if r in l, return part
ERegionCrossState regionCheck(const KRegion& regionl,
    const KRegion& regionr);

}