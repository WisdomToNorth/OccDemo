
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

#include "lc_struct.h"

#include "public_function.h"

namespace LeetCode
{
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::queue;

void printVec(const vector<int> &nums)
{
    for (auto a : nums)
    {
        std::cout << a << " ";
    }
    std::cout << '\n';
}
void printListNode(ListNode *list)
{
    while (list)
    {
        cout << list->val << ' ';
        list = list->next;
    }
    cout << endl;
}

ListNode *initListNode(std::vector<int> vec)
{
    ListNode *listnode = new ListNode(0);
    ListNode *current = listnode;
    for (auto a : vec)
    {
        // listnode->val = a;
        current->next = new ListNode(a);
        current = current->next;
    }
    return listnode->next;
}

template <typename T>
void printVec2D(vector<vector<T>> vec)
{
    cout << '[';
    for (auto a : vec)
    {
        cout << '[';
        for (auto b = a.cbegin(); b != a.cend(); ++b)
        {
            cout << *b;
            if (b != --a.cend())
                cout << ", ";
        }
        cout << ']';
    }
    cout << "]\n";
}

TreeNode *init_tree(vector<int> vec)
{
    TreeNode *res = new TreeNode(vec[0]);
    std::queue<TreeNode *> aux;
    aux.push(res);
    int cnt = 1;

    while (cnt < vec.size())
    {
        TreeNode *cur = aux.front();
        aux.pop();
        if (vec[cnt] == 0)
        {
            cur->left = nullptr;
        }
        else
        {
            cur->left = new TreeNode(vec[cnt]);
            aux.push(cur->left);
        }
        ++cnt;

        if (cnt == vec.size())
            return res;

        if (vec[cnt] == 0)
        {
            cur->right = nullptr;
        }
        else
        {
            cur->right = new TreeNode(vec[cnt]);
            aux.push(cur->right);
        }
        ++cnt;
    }
    return res;
}

int getValue(char ch)
{
    switch (ch)
    {
    case 'I': return 1;
    case 'V': return 5;
    case 'X': return 10;
    case 'L': return 50;
    case 'C': return 100;
    case 'D': return 500;
    case 'M': return 1000;
    default: return 0;
    }
}
double roundWith(double num, size_t cnt)
{
    int i = 1;
    for (size_t count = 0; count < cnt; ++count)
    {
        i *= 10;
    }

    double res = double(int(num * i)) / i;
    return res;
}
} // namespace LeetCode