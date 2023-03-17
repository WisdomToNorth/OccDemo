#include "RangeTree.h"

#include "multisort.h"

namespace KDebugger
{

BinSearchNode* KRangeTree::buildRangeTreeFromSortedVec(BinSearchNode* parent,
    PntsSorted2D& vec)//2D
{
    BinSearchNode* aux_tree_ = new BinSearchNode();
    BiSearch::buildFromSortedVec(aux_tree_, vec);

    if (vec.size() == 1)
    {
        parent->pnt_ = vec.getOnlyPnt();
        //appoint aux_node
        parent->aux_ = aux_tree_;
        return parent;
    }
    // BiSearch helper;


    PntsSorted2D r1;
    PntsSorted2D r2;
    //Caution: vec这里其实发生了变化，但是多数编译器在release版本下，
    //vec保持原值不变，使其得以正常运行。这里要研究一下怎么解决这个问题。
    KPt midpt = vec.getSubPntsByMidX(r1, r2);;

    //BinSearchNode* node = new BinSearchNode(midpt);
    parent->pnt_ = midpt;
    parent->left_ = buildRangeTreeFromSortedVec(new BinSearchNode(), r1);
    parent->right_ = buildRangeTreeFromSortedVec(new BinSearchNode(), r2);
    parent->aux_ = aux_tree_;

    return parent;
}

BinSearchNode* KRangeTree::buildRangeTree(std::vector<KPt>& pnts)
{
    if (pnts.empty())return nullptr;
    PntsSorted2D sorted_pnts(pnts);
    BinSearchNode* root = new BinSearchNode();
    return buildRangeTreeFromSortedVec(root, sorted_pnts);
}

// For 1D
// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
const BinSearchNode* KRangeTree::FindSplitNodeInX(const BinSearchNode* root,
    double leftnum, double rightnum)//1D
{
    const BinSearchNode* v = root;
    while (!v->isLeaf() && (rightnum <= v->pnt_.x || leftnum > v->pnt_.x))
    {
        if (rightnum <= v->pnt_.x)
            v = v->left_;
        else
            v = v->right_;
    }
    return v;
}

//search Range tree
//input: root of Range tree, to-search region
//output: vector of point in region
void KRangeTree::searchRangeTreeFromRoot(const BinSearchNode* root,
    const KRegion& region, std::vector<KPt>& res)
{
    const double& l = region.lb_.x;
    const double& r = region.ru_.x;
    const double& u = region.ru_.y;
    const double& b = region.lb_.y;

    const BinSearchNode* v_split = FindSplitNodeInX(root, l, r);

    if (v_split->isLeaf())
    {
        if (v_split->belongToRegion(region))
            res.emplace_back(v_split->pnt_);
        /* for (const auto& p : v_split->pnts_.pnts_xsorted_)
         {
             if (region.ptInRegion(p))
             {
                 res.emplace_back(p);
             }
         }*/
    }
    else
    {
        // find way to left
        const BinSearchNode* l_split = v_split->left_;

        while (!l_split->isLeaf())
        {
            if (l_split->pnt_.x >= l)
            { // report right tree
                BiSearch::oneDRangeQuery(l_split->right_->aux_, b, u, res);
                l_split = l_split->left_;
            }
            else
            {
                l_split = l_split->right_;
            }
        }
        if (l_split->belongToRegion(region))
            res.emplace_back(l_split->pnt_);
        /* if (l_split)
         {
             for (const auto& p : l_split->pnts_.pnts_xsorted_)
             {
                 if (region.ptInRegion(p))
                 {
                     res.emplace_back(p);
                 }
             }
         }*/

         // find way to right;
        const BinSearchNode* r_split = v_split->right_;

        while (!r_split->isLeaf())
        {
            if (r_split->pnt_.x < r)
            { // report right tree
                BiSearch::oneDRangeQuery(r_split->left_->aux_, b, u, res);
                r_split = r_split->right_;
            }
            else
            {
                r_split = r_split->left_;
            }
        }
        if (r_split->belongToRegion(region))
            res.emplace_back(r_split->pnt_);
        //if (r_split)
        //{
        //    for (const auto& p : r_split->pnts_.pnts_xsorted_)
        //    {
        //        if (region.ptInRegion(p))
        //        {
        //            res.emplace_back(p);
        //        }
        //    }
        //}

    }
}

}