#pragma once

#include "KPnt.h"
#include "KRegion.h"

namespace KDebugger
{

struct BinSearchNode
{
    BinSearchNode() : left_(nullptr), right_(nullptr), pnt_(KPt()), direction_(false) {}
    BinSearchNode(const KPt& x, bool direction, BinSearchNode* left, BinSearchNode* right)
        : left_(left), right_(right), pnt_(x), direction_(direction)
    {
    }
    BinSearchNode(const KPt& _pnt, bool direction) : pnt_(_pnt), direction_(direction) {}

    BinSearchNode* left_ = nullptr;
    BinSearchNode* right_ = nullptr;
    KPt pnt_;
    bool direction_; // 0 for hor; 1 for vert

public:
    bool isLeaf()const
    {
        if (left_ || right_)
            return false;
        else
            return true;
    }

    bool belongToRangeInX(int l, int r)const//in range [ )
    {
        return pnt_.x >= l && pnt_.x < r;
    }

    bool BinSearchNodeInRegion(const KRegion& region)//in range [ )
    {
        return region.ptInRegion(this->pnt_);
    }

    void printBinSearchTree(bool onlyX = false);

    void reportSubTree(std::vector<KPt>& subnodes);

private:
    void printBinSearchTree(const BinSearchNode* root,
        bool onlyX = false);

    void reportSubTree(BinSearchNode* root, std::vector<KPt>& subnodes);

};

}