#include "BinSearchNode.h"

#include<iostream>
#include <queue>
namespace KDebugger
{
void printBinSearchTree(BinSearchNode* root, bool onlyX)
{
    if (!root)return;
    std::queue <BinSearchNode*> q;
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