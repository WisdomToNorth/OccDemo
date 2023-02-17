#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include <assert.h>

#include "K_Pnt.h"

namespace KDebugger
{
struct KRegion
{
    KRegion(const KPt& lb, const KPt& ru) :left_bottom_(lb), right_up_(ru) {}
    KPt left_bottom_;
    KPt right_up_;
};

struct PntsSorted2D
{
    std::vector<KPt> pnts_xsorted_;
    std::vector<KPt> pnts_ysorted_;

    size_t size()
    {
        assert(pnts_xsorted_.size() == pnts_ysorted_.size());
        return pnts_xsorted_.size();
    }

    KPt getOnlyPnt()
    {
        assert(size() == 1);
        return pnts_xsorted_.front();
    }
};

struct BinSearchNode
{
    BinSearchNode() :pnt_(KPt()), left_(nullptr), right_(nullptr) {}
    BinSearchNode(const KPt& x, BinSearchNode* left, BinSearchNode* right)
        : pnt_(x), left_(left), right_(right)
    {}
    BinSearchNode(const KPt& _pnt) : pnt_(_pnt) {}

    KPt pnt_;
    bool direction_; // 0 for hor; 1 for vert
    BinSearchNode* left_;
    BinSearchNode* right_;

    bool isLeaf()const
    {
        if (left_ || right_)
            return false;
        else
            return true;
    }

    bool belongToRangeInX(int l, int r)
    {//todo: confirm boundry is right
        return pnt_.x >= l && pnt_.x <= r;
    }

    bool BinSearchNodeInRegion(const KRegion& region)
    {//todo: confirm boundry is right
        return (pnt_.x > region.left_bottom_.x) &&
            (pnt_.x < region.right_up_.x) &&
            (pnt_.y > region.left_bottom_.y) &&
            (pnt_.y < region.right_up_.y);
    }
};

bool BinSearchNodeInRegion(BinSearchNode* node, const KRegion& region)
{//todo: confirm boundry is right
    return (node->pnt_.x > region.left_bottom_.x) &&
        (node->pnt_.x < region.right_up_.x) &&
        (node->pnt_.y > region.left_bottom_.y) &&
        (node->pnt_.y < region.right_up_.y);
}

void reportSubTreeInX(BinSearchNode* root, std::vector<int>& subnodes)//1D
{
    if (root->isLeaf())
    {
        subnodes.emplace_back(root->pnt_.x);
    }
    if (root->left_)
        reportSubTreeInX(root->left_, subnodes);
    if (root->right_)
        reportSubTreeInX(root->right_, subnodes);
}

void reportSubTree(BinSearchNode* root, std::vector<KPt>& subnodes)//1D
{
    if (root->isLeaf())
    {
        subnodes.emplace_back(root->pnt_);
    }
    if (root->left_)
        reportSubTree(root->left_, subnodes);
    if (root->right_)
        reportSubTree(root->right_, subnodes);
}

template<typename Iterator>
BinSearchNode* buildFromSortedVec(BinSearchNode* parent, const std::vector<int>& vec, Iterator it, int cnt)
{
    if (cnt == 1)
    {
        parent->val = *it;
        return parent;
    }
    int leftcnt = cnt / 2;
    int rightcnt = cnt - leftcnt;
    auto mid_it = std::advance(it, leftcnt);
    parent->val = *mid_it;
    parent->left_ = buildFromSortedVec(vec, it, leftcnt)
        parent->right_ = buildFromSortedVec(vec, mid_it + 1, rightcnt);
    return parent;
}

BinSearchNode* buildBinSearchTree(std::vector<int>& vec)
{
    std::sort(vec.begin(), vec.end());
    int n = vec.size();
    BinSearchNode* root = new BinSearchNode();
    return buildFromSortedVec(root, vec, vec.begin(), vec.size());
}

// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
BinSearchNode* FindSplitNode(BinSearchNode* root,
    int leftnum, int rightnum)//1D
{
    BinSearchNode* v = root;
    while (!v->isLeaf() &&
        (rightnum <= v->pnt_.x || leftnum > v->pnt_.x))
    {
        if (rightnum <= v->pnt_.x)
            v = v->left_;
        else
            v = v->right_;
    }
    return v;
}

// input: bin search tree, to-search-range [x,x']
// output: vector<int> {numbers in to-search-range}
void oneDRangeQuery(BinSearchNode* root,
    int l, int r, std::vector<int>& res)
{
    BinSearchNode* v_split = FindSplitNode(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRangeInX(l, r))
            res.emplace_back(v_split->pnt_.x);
    }
    else
    {
        // find way to left
        BinSearchNode* l_split = v_split;
        while (!l_split->isLeaf())
        {
            if (l_split->left_->pnt_.x >= l)
            { // report right tree
                reportSubTreeInX(l_split->right_, res);
                l_split = l_split->left_;
            }
            else
            {
                l_split = l_split->right_;
            }
        }
        if (l_split->belongToRangeInX(l, r))
            res.emplace_back(l_split->pnt_.x);

        // find way to right;
        BinSearchNode* r_split = v_split;
        while (!r_split->isLeaf())
        {
            if (r_split->left_->pnt_.x < r)
            { // report right tree
                reportSubTreeInX(r_split->left_, res);
                r_split = r_split->right_;
            }
            else
            {
                r_split = r_split->left_;
            }
        }
        if (r_split->belongToRangeInX(l, r))
            res.emplace_back(r_split->pnt_.x);
    }
}

BinSearchNode* buildKDTreeFromSortedVec(const std::vector<KPt>& pnts)
{
    PntsSorted2D p_pnts;
    p_pnts.pnts_xsorted_ = pnts;
    p_pnts.pnts_ysorted_ = pnts;
    //sortx
    std::sort(p_pnts.pnts_xsorted_.begin(), p_pnts.pnts_xsorted_.end());
    // sorty
    std::sort(p_pnts.pnts_ysorted_.begin(), p_pnts.pnts_ysorted_.end(),
        [](KPt v1, KPt v2) -> bool
        {
            return v1.y < v2.y;
        });
    return buildKDTree(p_pnts, 0);
}

void splitPnts(PntsSorted2D& pnts, PntsSorted2D& p1s, PntsSorted2D& p2s, bool direction)
{
    int n = pnts.pnts_xsorted_.size();
    int mid = n / 2;
    if (direction)//vertical by mid x
    {
        //put into p1s
        p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(),
            pnts.pnts_xsorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin() + mid,
            pnts.pnts_xsorted_.end()); // mid to last

        double mid_x = (pnts.pnts_xsorted_.begin() + mid)->x;
        for (const KPt& pnt : pnts.pnts_ysorted_)
        {
            if (pnt.x <= mid_x)
            {
                p1s.pnts_ysorted_.emplace_back(pnt);
            }
            else
            {
                p2s.pnts_ysorted_.emplace_back(pnt);
            }
        }
    }
    else//horizon by mid y
    {
        //put into p1s
        p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
            pnts.pnts_ysorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
            pnts.pnts_ysorted_.end()); // mid to last

        double mid_y = (pnts.pnts_ysorted_.begin() + mid)->y;
        for (const KPt& pnt : pnts.pnts_xsorted_)
        {
            if (pnt.y <= mid_y)
            {
                p1s.pnts_xsorted_.emplace_back(pnt);
            }
            else
            {
                p2s.pnts_xsorted_.emplace_back(pnt);
            }
        }
    }
}

BinSearchNode* buildKDTree(PntsSorted2D pnts, int cur_depth)
{

    int n = pnts.pnts_xsorted_.size();
    if (n == 0)
        return nullptr;
    else if (n == 1)
        return new BinSearchNode(pnts.getOnlyPnt());
    else
    {
        PntsSorted2D p1s;
        PntsSorted2D p2s;
        int mid = n / 2;
        BinSearchNode* node = new BinSearchNode();
        if (cur_depth % 2) //vert
        {
            splitPnts(pnts, p1s, p2s, 1);
            node->direction_ = 1;
        }
        else//horizon
        {
            splitPnts(pnts, p1s, p2s, 0);

            node->direction_ = 0;
        }

        node->left_ = buildKDTree(p1s, cur_depth + 1);
        node->right_ = buildKDTree(p2s, cur_depth + 1);
    }
}
//declare
void searchKDTree(std::vector<PntsSorted2D>& res, BinSearchNode* root,
    const KRegion& cur_region, const KRegion& region);

//search KD tree
//input: root of kd-tree, to-search region
//output: vector of point in region
std::vector<PntsSorted2D> searchKDTreeFromRoot(BinSearchNode* root, const KRegion& region)
{
    std::vector<PntsSorted2D> res;

    KRegion init_region(KPt(INT8_MIN, INT8_MIN), KPt(INT8_MAX, INT8_MAX));
    searchKDTree(res, root, init_region, region);
}

enum class ERegionCrossState
{
    out, part, in
};

ERegionCrossState regionCheck(const KRegion& regionl, const KRegion& regionr)
{

    //todo
    return ERegionCrossState::part;
}

KRegion getNewRegion(BinSearchNode* node, const KRegion& cur_region)
{

}

void searchKDTree(std::vector<KPt>& res, BinSearchNode* root,
    const KRegion& cur_region, const KRegion& region)
{
    if (root->isLeaf())
    {
        if (root->BinSearchNodeInRegion(region))res.emplace_back(root->pnt_);
    }
    else
    {
        KRegion left_region = getNewRegion(root->left_, cur_region);
        auto check_res_l = regionCheck(left_region, region);
        if (check_res_l == ERegionCrossState::in)
        {
            reportSubTree(root->left_, res);
        }
        else if (check_res_l == ERegionCrossState::part)
        {
            searchKDTree(res, root->left_, left_region, region);
        }

        KRegion right_region = getNewRegion(root->right_, cur_region);
        auto check_res_r = regionCheck(right_region, region);
        if (check_res_r == ERegionCrossState::in)
        {
            reportSubTree(root->right_, res);
        }
        else if (check_res_r == ERegionCrossState::part)
        {
            searchKDTree(res, root->right_, right_region, region);
        }
    }
}
}