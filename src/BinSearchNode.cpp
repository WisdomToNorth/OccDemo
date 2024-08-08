#include "BinSearchNode.h"

#include <iostream>
#include <queue>
namespace KDebugger
{

void BinSearchNode::printBinSearchTree(bool onlyY) const
{
    printBinSearchTree(this, onlyY);
}

void BinSearchNode::reportSubTree(std::vector<KPt> &subnodes) const
{
    reportSubTree(this, subnodes);
}

void BinSearchNode::reportSubTree(const BinSearchNode *root,
                                  std::vector<KPt> &subnodes) const // 1D
{
    // std::cout << "\n%%%%report subtree: \n";
    // printBinSearchTree(root);
    // std::cout << "\n%%%%report subtree end \n";
    if (!root) return;
    if (root->isLeaf())
    {
        subnodes.emplace_back(root->pnt_);
        // for (auto& p : root->pnts_.pnts_xsorted_)
        //     subnodes.push_back(p);
    }
    if (root->left_)
        reportSubTree(root->left_, subnodes);
    if (root->right_)
        reportSubTree(root->right_, subnodes);
}

void BinSearchNode::printBinSearchTree(const BinSearchNode *root, bool onlyY) const
{
    if (!root) return;
    std::queue<const BinSearchNode *> q;
    q.push(root);
    while (!q.empty())
    {
        int cur_level_size = q.size();
        for (int i = 1; i <= cur_level_size; ++i)
        {
            auto node = q.front();
            q.pop();

            if (onlyY)
                node->pnt_.printY();
            else
                node->pnt_.print();
            std::cout << '\t';
            if (node->left_) q.push(node->left_);
            if (node->right_) q.push(node->right_);
        }
        std::cout << std::endl;
    }
}

} // namespace KDebugger