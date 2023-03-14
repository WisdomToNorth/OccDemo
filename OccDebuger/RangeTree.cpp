﻿#include "RangeTree.h"

#include "multisort.h"

namespace KDebugger
{

BinSearchNode* KRangeTree::buildRangeTreeFromSortedVec(PntsSorted2D& vec)//2D
{

    BiSearch helper;
    BinSearchNode* aux_tree_ = helper.buildFromSortedVec(vec);

    //std::vector<PntsSorted2D> aux;
    PntsSorted2D r1;// = aux.front();
    PntsSorted2D r2;// = aux.back();
    KPt midpt = vec.getSubPntsByMidX(r1, r2);;

    //PntsSorted2D& r1 = aux.front();
    //PntsSorted2D& r2 = aux.back();
    BinSearchNode* node;

    if (r1.empty() || r2.empty())
    {
        node = new BinSearchNode(vec);
        node->aux_ = aux_tree_;
    }
    else
    {
        node = new BinSearchNode(midpt);
        node->left_ = buildRangeTreeFromSortedVec(r1);
        node->right_ = buildRangeTreeFromSortedVec(r2);
        node->aux_ = aux_tree_;
    }
    return node;
}

BinSearchNode* KRangeTree::buildRangeTree(std::vector<KPt>& pnts)
{
    if (pnts.empty())return nullptr;
    PntsSorted2D sorted_pnts(pnts);
    return buildRangeTreeFromSortedVec(sorted_pnts);
}

// For 1D
// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
const BinSearchNode* KRangeTree::FindSplitNodeInX(const BinSearchNode* root,
    double leftnum, double rightnum)//1D
{
    if (!root)return nullptr;
    const BinSearchNode* v = root;
    while (!v->isLeaf() && (rightnum <= v->pnt_.x || leftnum > v->pnt_.x))
    {
        if (rightnum <= v->pnt_.x)
            v = v->left_;
        else
            v = v->right_;
    }
    //std::cout << "\n####spliter of [" << leftnum << ", " << rightnum << "):\n";
    //printBinSearchTree(v, true);
    //std::cout << std::endl;
    return v;
}

//search Range tree
//input: root of Range tree, to-search region
//output: vector of point in region
void KRangeTree::searchRangeTreeFromRoot(BinSearchNode* root,
    const KRegion& region, std::vector<KPt>& res)
{
    if (!root)return;
    double l = region.lb_.x;
    double r = region.ru_.x;
    double u = region.ru_.y;
    double b = region.lb_.y;
    BiSearch helper;

    const BinSearchNode* v_split = FindSplitNodeInX(root, l, r);
    if (!v_split)return;

    if (v_split->isLeaf())
    {
        for (auto& p : v_split->pnts_.pnts_xsorted_)
        {
            if (region.ptInRegion(p))
            {
                res.emplace_back(p);
            }
        }
    }
    else
    {
        // find way to left
        const BinSearchNode* l_split = v_split->left_;

        while (l_split && !l_split->isLeaf())
        {
            if (l_split->pnt_.x >= l)
            { // report right tree
                helper.oneDRangeQuery(l_split->right_->aux_, b, u, res);
                l_split = l_split->left_;
            }
            else
            {
                l_split = l_split->right_;
            }
        }
        if (l_split)
        {
            for (auto& p : l_split->pnts_.pnts_xsorted_)
            {
                if (region.ptInRegion(p))
                {
                    res.emplace_back(p);
                }
            }
        }
        // find way to right;
        const BinSearchNode* r_split = v_split->right_;

        while (r_split && !r_split->isLeaf())
        {
            if (r_split->pnt_.x < r)
            { // report right tree
                helper.oneDRangeQuery(r_split->left_->aux_, b, u, res);
                r_split = r_split->right_;
            }
            else
            {
                r_split = r_split->left_;
            }
        }
        if (r_split)
        {
            for (auto& p : r_split->pnts_.pnts_xsorted_)
            {
                if (region.ptInRegion(p))
                {
                    res.emplace_back(p);
                }
            }
        }

    }
}

}