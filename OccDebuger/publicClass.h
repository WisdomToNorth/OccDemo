#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <stack>
#include <deque>
#include <queue>

using std::cout; using std::endl; using std::vector; using std::string;
using std::queue;
static void printvec(const vector<int>& nums)
{
    for (auto a : nums)
    {
        std::cout << a << " ";
    }
    std::cout << '\n';
}

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}

};

class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};


struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}

};

static void printListnode(ListNode* list)
{
    while (list)
    {
        cout << list->val << ' ';
        list = list->next;
    }
    cout << endl;
}

static ListNode* init_ListNode(std::vector<int> vec)
{
    ListNode* listnode = new ListNode(0);
    ListNode* current = listnode;
    for (auto a : vec)
    {
        //listnode->val = a;
        current->next = new ListNode(a);
        current = current->next;
    }
    return listnode->next;
}

template<typename T>
static void printvecvec(vector<vector<T>> vec)
{
    cout << '[';
    for (auto a : vec)
    {
        cout << '[';
        for (auto b = a.cbegin(); b != a.cend(); ++b)
        {
            cout << *b;
            if (b != --a.cend())cout << ", ";
        }
        cout << ']';
    }
    cout << "]\n";
}

static TreeNode* init_tree(vector<int> vec)
{
    TreeNode* res = new TreeNode(vec[0]);
    std::queue<TreeNode*> aux;
    aux.push(res);
    int cnt = 1;

    while (cnt < vec.size())
    {

        TreeNode* cur = aux.front();
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

        if (cnt == vec.size())return res;

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
    switch (ch) {
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