#include "BiSearch.h"

#include "multisort.h"

namespace KDebugger
{
BiSearch::BiSearch() {}

// For 1D
template<typename Iterator>
BinSearchNode* BiSearch::buildFromSortedVec(BinSearchNode* parent,
    const std::vector<KPt>& vec, Iterator it, int cnt)//1D
{

    assert(cnt > 0);
    if (cnt == 1)
    {
        parent->pnt_ = *it;
        return parent;
    }
    //计数的时候添加哨兵会更容易
    // -,1,2,3
    // -,1,2,3,4


    int cnt_P1 = cnt + 1;
    int leftcnt = cnt_P1 / 2;
    int rightcnt = cnt - leftcnt;
    auto mid_it = it;
    std::advance(mid_it, leftcnt - 1);

    parent->pnt_ = *mid_it;
    parent->left_ = buildFromSortedVec(new BinSearchNode(),
        vec, it, leftcnt);
    parent->right_ = buildFromSortedVec(new BinSearchNode(),
        vec, mid_it + 1, rightcnt);
    return parent;
}

// For 1D
BinSearchNode* BiSearch::buildBinSearchTree(std::vector<KPt>& vec)//1D
{
    Sort_YS(vec.begin(), vec.end());
    //std::cout << "\nAfter sort:" << std::endl;
    //printPntVec(vec);

    BinSearchNode* root = new BinSearchNode();
    return buildFromSortedVec(root, vec, vec.begin(), vec.size());
}

// For 1D
// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
const BinSearchNode* BiSearch::FindSplitNode(const BinSearchNode* root,
    double leftnum, double rightnum)//1D
{
    const BinSearchNode* v = root;
    while (!v->isLeaf() && (rightnum <= v->pnt_.y || leftnum > v->pnt_.y))
    {
        if (rightnum <= v->pnt_.y)
            v = v->left_;
        else
            v = v->right_;
    }
    //std::cout << "\n####spliter of [" << leftnum << ", " << rightnum << "):\n";
    //printBinSearchTree(v, true);
    //std::cout << std::endl;
    return v;
}

// For 1D
// input: bin search tree, to-search-range [x,x']
// output: vector<int> {numbers in to-search-range}
void BiSearch::oneDRangeQuery(const BinSearchNode* root,
    double l, double r, std::vector<KPt>& res)
{
    const BinSearchNode* v_split = FindSplitNode(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRangeInY(l, r))
            res.emplace_back(v_split->pnt_);
    }
    else
    {
        // find way to left
        const BinSearchNode* l_split = v_split->left_;
        while (!l_split->isLeaf())
        {
            if (l_split->pnt_.y >= l)
            { // report right tree
                l_split->right_->reportSubTree(res);
                l_split = l_split->left_;
            }
            else
            {
                l_split = l_split->right_;
            }
        }
        if (l_split->belongToRangeInY(l, r))
            res.emplace_back(l_split->pnt_);

        // find way to right;
        const BinSearchNode* r_split = v_split->right_;
        while (!r_split->isLeaf())
        {
            if (r_split->pnt_.y < r)
            { // report right tree
                r_split->left_->reportSubTree(res);
                r_split = r_split->right_;
            }
            else
            {
                r_split = r_split->left_;
            }
        }
        if (r_split->belongToRangeInY(l, r))
            res.emplace_back(r_split->pnt_);
    }
}
}