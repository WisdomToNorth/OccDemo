#pragma once

struct TreeNode
{
    int val;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode() : val(0), left_(nullptr), right_(nullptr) {}
    TreeNode(int x) : val(x), left_(nullptr), right_(nullptr) {}
    TreeNode(int x, TreeNode* left_, TreeNode* right_) :
        val(x), left_(left_), right_(right_)
    {}
};

struct Node
{
public:
    int val;
    Node* left_;
    Node* right_;
    Node* next;

    Node() : val(0), left_(NULL), right_(NULL), next(NULL) {}

    Node(int _val) : val(_val), left_(NULL), right_(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left_(_left), right_(_right), next(_next)
    {}
};


struct ListNode
{
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}

};
