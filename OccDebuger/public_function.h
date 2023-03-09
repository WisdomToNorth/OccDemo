#pragma once

#include <vector>

namespace KDebugger
{
struct ListNode;
struct TreeNode;

void printVec(const std::vector<int>& nums);

void printListNode(ListNode* list);

ListNode* initListNode(std::vector<int> vec);

template<typename T>
void printVec2D(std::vector<std::vector<T>> vec);

TreeNode* init_tree(std::vector<int> vec);

int getValue(char ch);

double roundWith(double num, size_t cnt);


}