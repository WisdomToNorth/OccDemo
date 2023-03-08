#include "BinSearchNode.h"

#include<iostream>
#include <queue>
namespace KDebugger
{

void BinSearchNode::printBinSearchTree(bool onlyX)
{
    printBinSearchTree(this, onlyX);
}

void BinSearchNode::reportSubTree(std::vector<KPt>& subnodes)
{
    reportSubTree(this, subnodes);
}

void BinSearchNode::reportSubTree(BinSearchNode* root,
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

void BinSearchNode::printBinSearchTree(const BinSearchNode* root, bool onlyX)
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