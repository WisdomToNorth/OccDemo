﻿#include <QtWidgets/QApplication>
#include <iostream>

#include "MainWindowOcc.h"

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     KDebugger::MainWindowOcc w;
//     w.show();

//     return a.exec();
// }

#include "lc_struct.h"
#include <algorithm>
#include <queue>

using namespace std;
using namespace LeetCode;

struct SubtreeStatus
{
    int selected;
    int notSelected;
};

class Solution
{
public:
    SubtreeStatus dfs(TreeNode *node)
    {
        if (!node)
        {
            return {0, 0};
        }
        auto l = dfs(node->left);
        auto r = dfs(node->right);
        int selected = node->val + l.notSelected + r.notSelected;
        int notSelected = max(l.selected, l.notSelected) + max(r.selected, r.notSelected);
        return {selected, notSelected};
    }

    int rob(TreeNode *root)
    {
        auto rootStatus = dfs(root);
        return max(rootStatus.selected, rootStatus.notSelected);
    }
};

int main()
{
    Solution s;
    return 0;
}