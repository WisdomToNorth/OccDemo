#include "RangeTree.h"

#include "multisort.h"

namespace KDebugger
{
BinSearchNode* KRangeTree::buildRangeTree(std::vector<KPt>& pnts)
{

    Sort_XS(pnts.begin(), pnts.end());

    size_t n = pnts.size();
    BinSearchNode* root = new BinSearchNode();
    return buildRangeTreeFromSortedVec(root, pnts, pnts.begin(), pnts.size());
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
    double l = region.lb_.x;
    double r = region.ru_.x;
    double u = region.ru_.y;
    double b = region.lb_.y;
    BiSearch helper;

    const BinSearchNode* v_split = FindSplitNodeInX(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRegion(region))
            res.emplace_back(v_split->pnt_);
    }
    else
    {
        // find way to left
        const BinSearchNode* l_split = v_split->left_;
        while (!l_split->isLeaf())
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
        if (l_split->belongToRegion(region))
            res.emplace_back(l_split->pnt_);

        // find way to right;
        const BinSearchNode* r_split = v_split->right_;
        while (!r_split->isLeaf())
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
        if (r_split->belongToRegion(region))
            res.emplace_back(r_split->pnt_);
    }
}

template<typename Iterator>
BinSearchNode* KRangeTree::buildRangeTreeFromSortedVec(BinSearchNode* parent,
    const std::vector<KPt>& vec, Iterator it, size_t cnt)//1D
{
    std::vector<KPt> pnts_y(it, it + cnt);
    BiSearch helper;
    BinSearchNode* aux_tree_ = helper.buildBinSearchTree(pnts_y);

    assert(cnt > 0);
    if (cnt == 1)
    {
        parent->pnt_ = *it;
        //appoint aux_node
        parent->aux_ = aux_tree_;
        return parent;
    }
    //else

    //计数的时候添加哨兵会更容易
    // -,1,2,3
    // -,1,2,3,4
    int cnt_P1 = cnt + 1;
    int leftcnt = cnt_P1 / 2;
    int rightcnt = cnt - leftcnt;
    auto mid_it = it;
    std::advance(mid_it, leftcnt - 1);

    parent->pnt_ = *mid_it;
    parent->aux_ = aux_tree_;
    parent->left_ = buildRangeTreeFromSortedVec(new BinSearchNode(),
        vec, it, leftcnt);
    parent->right_ = buildRangeTreeFromSortedVec(new BinSearchNode(),
        vec, mid_it + 1, rightcnt);
    return parent;
}
}