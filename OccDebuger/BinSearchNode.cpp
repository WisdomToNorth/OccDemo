#include "BinSearchNode.h"

#include<iostream>
#include <queue>
namespace KDebugger
{


void reportSubTree(BinSearchNode* root,
    std::vector<KPt>& subnodes)//1D
{
    //std::cout << "\n%%%%report subtree: \n";
    //printBinSearchTree(root);
    //std::cout << "\n%%%%report subtree end \n";
    if (root->isLeaf())
    {
        subnodes.emplace_back(root->pnt_);
    }
    if (root->left_)
        reportSubTree(root->left_, subnodes);
    if (root->right_)
        reportSubTree(root->right_, subnodes);
}

// For 1D
template<typename Iterator>
BinSearchNode* buildFromSortedVec(BinSearchNode* parent,
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
    parent->left_ = buildFromSortedVec(new BinSearchNode(), vec, it, leftcnt);
    parent->right_ = buildFromSortedVec(new BinSearchNode(), vec, mid_it + 1, rightcnt);
    return parent;
}

// For 1D
BinSearchNode* buildBinSearchTree(std::vector<KPt>& vec)//1D
{
    std::sort(vec.begin(), vec.end(),
        [](KPt& pt1, KPt& pt2)
        {
            return pt1.x < pt2.x;
        });
    //std::cout << "\nAfter sort:" << std::endl;
    //printPntVec(vec);

    int n = vec.size();
    BinSearchNode* root = new BinSearchNode();
    return buildFromSortedVec(root, vec, vec.begin(), vec.size());
}

// For 1D
// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
const BinSearchNode* FindSplitNode(const BinSearchNode* root,
    double leftnum, double rightnum)//1D
{
    const BinSearchNode* v = root;
    while (!v->isLeaf() && (rightnum <= v->pnt_.x || leftnum > v->pnt_.x))
    {
        if (rightnum < v->pnt_.x)
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
void oneDRangeQuery(const BinSearchNode* root,
    double l, double r, std::vector<KPt>& res)
{
    const BinSearchNode* v_split = FindSplitNode(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRangeInX(l, r))
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
        const BinSearchNode* r_split = v_split->right_;
        while (!r_split->isLeaf())
        {
            if (r_split->pnt_.x < r)
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

void printBinSearchTree(const BinSearchNode* root, bool onlyX)
{
    if (!root)return;
    std::queue <const BinSearchNode*> q;
    q.push(root);
    while (!q.empty())
    {
        int cur_level_size = q.size();
        for (int i = 1; i <= cur_level_size; ++i)
        {
            auto node = q.front();
            q.pop();

            if (onlyX)node->pnt_.printX();
            else node->pnt_.print();
            std::cout << '\t';
            if (node->left_)q.push(node->left_);
            if (node->right_)q.push(node->right_);
        }
        std::cout << std::endl;
    }
}

}