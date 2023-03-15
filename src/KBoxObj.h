#pragma once

#include "KPnt.h"
#include "KBox.h"
#include "BoundingBox.h"
#include "KRegion.h"
#include <vector>

namespace KDebugger
{
struct KBoxObj
{
    KBoxObj(const KBox& box) :
        loc_(box.getPt())
    {
        const KBoundingBox& abb = box.getBoundingbox();
        corners_.push_back(KPt(abb.lb_, &loc_));
        corners_.emplace_back(KPt(abb.lb_.x, abb.ru_.y, &loc_));
        corners_.emplace_back(KPt(abb.ru_.x, abb.ru_.y, &loc_));
        corners_.emplace_back(KPt(abb.ru_, &loc_));
    }

    const std::vector<KPt>& getBoxPt()
    {
        return corners_;
    };
    KRegion getRegion()
    {
        return KRegion(corners_[0],corners_[3]);
    }

    KPt loc_;
    std::vector<KPt> corners_;
};
}
