#pragma once

#include "K_Pnt.h"

namespace KDebugger
{

struct BinSearchNode
{
    BinSearchNode() :pnt_(KPt()), left_(nullptr), right_(nullptr) {}
    BinSearchNode(const KPt& x, BinSearchNode* left, BinSearchNode* right)
        : pnt_(x), left_(left), right_(right)
    {}
    BinSearchNode(const KPt& _pnt) : pnt_(_pnt) {}

    BinSearchNode* left_ = nullptr;
    BinSearchNode* right_ = nullptr;
    //int val = 0;
    KPt pnt_;
    bool direction_ = 0; // 0 for hor; 1 for vert

    bool isLeaf()const
    {
        if (left_ || right_)
            return false;
        else
            return true;
    }

    bool belongToRangeInX(int l, int r)const
    {//todo: confirm boundry is right
        return pnt_.x > l && pnt_.x <= r;
    }

    bool BinSearchNodeInRegion(const KRegion& region)
    {//todo: confirm boundry is right
        return (pnt_.x > region.left_bottom_.x) &&
            (pnt_.x < region.right_up_.x) &&
            (pnt_.y > region.left_bottom_.y) &&
            (pnt_.y < region.right_up_.y);
    }
};

}