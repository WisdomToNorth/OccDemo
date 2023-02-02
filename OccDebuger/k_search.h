#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
struct k_Pnt
{
    double x;
    double y;
};

struct TreeNode
{
    TreeNode() :pnt_(k_Pnt()), left(nullptr), right(nullptr) {}
    //TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(const k_Pnt& x, TreeNode* left, TreeNode* right) : pnt_(x), left(left), right(right) {}
    TreeNode(const k_Pnt& _pnt) : pnt_(_pnt) {}
    //int val;
    k_Pnt pnt_;
    bool direction_; // 0 for hor; 1 for vert
    TreeNode* left;
    TreeNode* right;

    bool isLeaf()const
    {
        if (left || right)
            return false;
        else
            return true;
    }
    bool belongToRange(int l, int r)
    {
        return pnt_.x >= l && pnt_.x <= r;
    }
};

void reportSubTree(TreeNode* root, std::vector<int>& subnodes)//1D
{
    if (root->isLeaf())
    {
        subnodes.emplace_back(root->pnt_.x);
    }
    if (root->left)
        reportSubTree(root->left, subnodes);
    if (root->right)
        reportSubTree(root->right, subnodes);
}

template<typename Iterator>
TreeNode* buildFromSortedVec(TreeNode* parent, const std::vector<int>& vec, Iterator it, int cnt)
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
    parent->left = buildFromSortedVec(vec, it, leftcnt)
        parent->right = buildFromSortedVec(vec, mid_it + 1, rightcnt);
    return parent;
}

TreeNode* buildBinSearchTree(std::vector<int>& vec)
{
    std::sort(vec.begin(), vec.end());
    int n = vec.size();
    TreeNode* root;
    return buildFromSortedVec(root, vec, vec.begin(), vec.size());
}

// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
TreeNode* FindSplitNode(TreeNode* root,
    int leftnum, int rightnum)//1D
{
    TreeNode* v = root;
    while (!v->isLeaf() &&
        (rightnum <= v->pnt_.x || leftnum > v->pnt_.x))
    {
        if (rightnum <= v->pnt_.x)
            v = v->left;
        else
            v = v->right;
    }
    return v;
}

// input: bin search tree, to-search-range [x,x']
// output: vector<int> {numbers in to-search-range}
void oneDRangeQuery(TreeNode* root,
    int l, int r, std::vector<int>& res)
{
    TreeNode* v_split = FindSplitNode(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRange(l, r))
            res.emplace_back(v_split->pnt_.x);
    }
    else
    {
        // find way to left
        TreeNode* l_split = v_split;
        while (!l_split->isLeaf())
        {
            if (l_split->left->pnt_.x >= l)
            { // report right tree
                reportSubTree(l_split->right, res);
                l_split = l_split->left;
            }
            else
            {
                l_split = l_split->right;
            }
        }
        if (l_split->belongToRange(l, r))
            res.emplace_back(l_split->pnt_.x);

        // find way to right;
        TreeNode* r_split = v_split;
        while (!r_split->isLeaf())
        {
            if (r_split->left->pnt_.x < r)
            { // report right tree
                reportSubTree(r_split->left, res);
                r_split = r_split->right;
            }
            else
            {
                r_split = r_split->left;
            }
        }
        if (r_split->belongToRange(l, r))
            res.emplace_back(r_split->pnt_.x);
    }
}

struct PntsSorted2D
{
    std::vector<k_Pnt> pnts_xsorted_;
    std::vector<k_Pnt> pnts_ysorted_;

    void cleanUpAndPrepare()
    {
        if (pnts_xsorted_.empty())
        {
            pnts_xsorted_ = pnts_ysorted_;
            std::sort(pnts_xsorted_.begin(), pnts_xsorted_.end());
        }

        else if (pnts_ysorted_.empty())
        {
            pnts_ysorted_ = pnts_xsorted_;
            std::sort(pnts_ysorted_.begin(), pnts_ysorted_.end(),
                [](k_Pnt v1, k_Pnt v2) -> bool
                {
                    return v1.y < v2.y;
                });
        }
    }

    k_Pnt getFront()
    {
        if (pnts_xsorted_.empty())
        {
            std::cerr << "empty PntsSorted2D";
            return k_Pnt();
        }
        else return pnts_xsorted_.front();
    }
};

TreeNode* buildKDTree(const std::vector<k_Pnt>& pnts)
{
    PntsSorted2D p_pnts;
    p_pnts.pnts_xsorted_ = pnts;
    p_pnts.pnts_ysorted_ = pnts;
    // sortx
    // sorty
    return buildKDTree(p_pnts, 0);
}

TreeNode* buildKDTree(PntsSorted2D pnts, int cur_depth)
{

    int n = pnts.pnts_xsorted_.size();
    if (n == 0)
        return nullptr;
    else if (n == 1)
        return new TreeNode(pnts.getFront());
    else
    {
        PntsSorted2D p1s;
        PntsSorted2D p2s;
        int mid = n / 2;
        TreeNode* node;
        if (cur_depth % 2) //
        {
            p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(),
                pnts.pnts_xsorted_.begin() + mid); // 0 to mid
            p1s.cleanUpAndPrepare();
            p2s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin() + mid,
                pnts.pnts_xsorted_.end()); // mid to last
            p2s.cleanUpAndPrepare();
            //todo: cleanup p1s and p2s in O(N)
            node->direction_ = 1;
        }
        else
        {
            p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
                pnts.pnts_ysorted_.begin() + mid); // 0 to mid
            p1s.cleanUpAndPrepare();
            p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
                pnts.pnts_ysorted_.end()); // mid to last
            p2s.cleanUpAndPrepare();

            node->direction_ = 0;
        }

        node->left = buildKDTree(p1s, cur_depth + 1);
        node->right = buildKDTree(p2s, cur_depth + 1);
    }
}