#pragma once

#include "KPnt.h"
#include "KRegion.h"

namespace KDebugger
{

struct BinSearchNode
{
    BinSearchNode() :
        left_(nullptr), right_(nullptr), pnt_(KPt()), direction_(false)
    {}
    BinSearchNode(const KPt &x, bool direction, BinSearchNode *left, BinSearchNode *right) :
        left_(left), right_(right), pnt_(x), direction_(direction)
    {
    }
    BinSearchNode(const KPt &_pnt, bool direction) :
        pnt_(_pnt), direction_(direction)
    {}
    BinSearchNode(const KPt &_pnt) :
        pnt_(_pnt)
    {}
    BinSearchNode(const PntsSorted2D &_pnt) :
        pnt_(KPt(-1, -1)), pnts_(_pnt)
    {}

    BinSearchNode *left_ = nullptr;
    BinSearchNode *right_ = nullptr;
    BinSearchNode *aux_ = nullptr;

    KPt pnt_;
    PntsSorted2D pnts_;
    bool direction_ = false; // 0 for hor; 1 for vert

public:
    bool isLeaf() const
    {
        if (left_ || right_)
            return false;
        else
            return true;
    }

    bool belongToRangeInX(double l, double r) const // in range [ )
    {
        return pnt_.x >= l && pnt_.x < r;
    }

    bool belongToRangeInY(double b, double u) const // in range [ )
    {
        return pnt_.y >= b && pnt_.y < u;
    }

    bool belongToRegion(const KRegion &region) const // in range [ )
    {
        return region.ptInRegion(this->pnt_);
    }

    void printBinSearchTree(bool onlyY = false) const;

    void reportSubTree(std::vector<KPt> &subnodes) const;

private:
    void printBinSearchTree(const BinSearchNode *root,
                            bool onlyX = false) const;

    void reportSubTree(const BinSearchNode *root, std::vector<KPt> &subnodes) const;
};
} // namespace KDebugger