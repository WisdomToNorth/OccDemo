﻿#pragma once

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
        loc_.obj_ptr_ = this;
        const KBoundingBox& abb = box.getBoundingbox();
        KPt p1(abb.lb_, &loc_);
        KPt p2(abb.lb_.x, abb.ru_.y, &loc_);
        KPt p3(abb.ru_.x, abb.ru_.y, &loc_);
        KPt p4(abb.ru_, &loc_);

        corners_.emplace_back(p1);
        corners_.emplace_back(p2);
        corners_.emplace_back(p3);
        corners_.emplace_back(p4);
    }
    //KBoxObj(const KBoxObj& rhs) = delete;
    KBoxObj(const KBoxObj& rhs)
    {
        loc_ = rhs.loc_;

        for (auto& p : rhs.corners_)
        {
            corners_.emplace_back(p);
        }
    }

    const std::vector<KPt>& getBoxPt()const
    {
        return corners_;
    };
    KRegion getRegion()
    {
        return KRegion(corners_[0], corners_[3]);
    }

    KPt loc_;
    std::vector<KPt> corners_;
};
}
