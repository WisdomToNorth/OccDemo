#pragma once

#include "K_Pnt.h"
#include "KRegion.h"

namespace KDebugger
{

struct BinSearchNode
{
    BinSearchNode() :pnt_(KPt()), left_(nullptr), right_(nullptr) {}
    BinSearchNode(const KPt& x, BinSearchNode* left, BinSearchNode* right)
        : pnt_(x), left_(left), right_(right)
    {
    }
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

    bool belongToRangeInX(int l, int r)const//in range [ )
    {
        return pnt_.x >= l && pnt_.x < r;
    }

    bool BinSearchNodeInRegion(const KRegion& region)
    {//todo: confirm boundry is right
        return region.ptInRegion(this->pnt_);
    }
};

void printBinSearchTree(const BinSearchNode* root,
    bool onlyX = false);
void reportSubTree(BinSearchNode* root, std::vector<KPt>& subnodes);

// For 1D
template<typename Iterator>
BinSearchNode* buildFromSortedVec(BinSearchNode* parent,
    const std::vector<KPt>& vec, Iterator it, int cnt);

// For 1D
BinSearchNode* buildBinSearchTree(std::vector<KPt>& vec);

// For 1D
// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
const BinSearchNode* FindSplitNode(const BinSearchNode* root,
    double leftnum, double rightnum);

// For 1D
// input: bin search tree, to-search-range [x,x']
// output: vector<int> {numbers in to-search-range}
void oneDRangeQuery(const BinSearchNode* root,
    double l, double r, std::vector<KPt>& res);
}