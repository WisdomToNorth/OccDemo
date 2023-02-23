
#include "kd_search.h"
#include "DataGenerator.h"

namespace KDebugger
{


KDTree::KDTree(DataGenerator* generator) :DataObserver(generator)
{
    updateData();
}
void KDTree::updateData()
{
    data_generator_->getPtData(buf_);
}

//
//void KDTree::reportSubTreeInX(BinSearchNode* root, std::vector<double>& subnodes)//1D
//{
//    if (root->isLeaf())
//    {
//        subnodes.emplace_back(root->pnt_.x);
//    }
//    if (root->left_)
//        reportSubTreeInX(root->left_, subnodes);
//    if (root->right_)
//        reportSubTreeInX(root->right_, subnodes);
//}

void KDTree::getOneDRange(double l, double r)
{
    BinSearchNode* binsearch_1d_ = buildBinSearchTree(buf_);
    std::vector<KPt> res;
    oneDRangeQuery(binsearch_1d_, l, r, res);
    for (const auto& pnt : res)
    {
        pnt.print();
    }

}

void KDTree::reportSubTree(BinSearchNode* root, std::vector<KPt>& subnodes)//1D
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
BinSearchNode* KDTree::buildFromSortedVec(BinSearchNode* parent,
    const std::vector<KPt>& vec, Iterator it, int cnt)//1D
{
    if (cnt == 1)
    {
        parent->pnt_ = *it;
        return parent;
    }
    int leftcnt = cnt / 2;
    int rightcnt = cnt - leftcnt;
    auto mid_it = it;
    std::advance(mid_it, leftcnt);

    parent->pnt_ = *it;
    parent->left_ = buildFromSortedVec(parent, vec, it, leftcnt);
    parent->right_ = buildFromSortedVec(parent, vec, mid_it + 1, rightcnt);
    return parent;
}

BinSearchNode* KDTree::buildBinSearchTree(std::vector<KPt>& vec)//1D
{
    std::sort(vec.begin(), vec.end(),
        [](KPt& pt1, KPt& pt2)
        {
            return pt1.x < pt2.x;
        });
    int n = vec.size();
    BinSearchNode* root = new BinSearchNode();
    return buildFromSortedVec(root, vec, vec.begin(), vec.size());
}



// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
BinSearchNode* KDTree::FindSplitNode(BinSearchNode* root,
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
void KDTree::oneDRangeQuery(BinSearchNode* root,
    double l, double r, std::vector<KPt>& res)
{
    BinSearchNode* v_split = FindSplitNode(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRangeInX(l, r))
            res.emplace_back(v_split->pnt_);
    }
    else
    {
        // find way to left
        BinSearchNode* l_split = v_split;
        while (!l_split->isLeaf())
        {
            if (l_split->left_->pnt_.x >= l)
            { // report right tree
                reportSubTree(l_split->right_, res);
                l_split = l_split->left_;
            }
            else
            {
                l_split = l_split->right_;
            }
        }
        if (l_split->belongToRangeInX(l, r))
            res.emplace_back(l_split->pnt_);

        // find way to right;
        BinSearchNode* r_split = v_split;
        while (!r_split->isLeaf())
        {
            if (r_split->left_->pnt_.x < r)
            { // report right tree
                reportSubTree(r_split->left_, res);
                r_split = r_split->right_;
            }
            else
            {
                r_split = r_split->left_;
            }
        }
        if (r_split->belongToRangeInX(l, r))
            res.emplace_back(r_split->pnt_);
    }
}
}