#pragma once
#include <iostream>
#include <vector>

namespace MyOwnData
{
using std::vector;
vector<int> vec1{4, 5, 8, 9};
vector<int> vec2{9, 8, 1, 0, 1, 9, 4, 0, 4, 1};

vector<vector<int>> vecs{
    {},
    {1},
    {1, 0, 2, 3},
    {1, 2, 3, 0, 5, 6, 7, 8, 11},
};
ListNode *listnode1 = init_ListNode(vec1);
ListNode *listnode2 = init_ListNode(vec2);
vector<vector<int>> matrix1 = {{1, 1, 0, 0}, {1, 1, 0, 1}, {0, 1, 1, 0}, {1, 1, 0, 1}};
vector<vector<int>> matrix2 = {{1, 0, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 1, 1}};

vector<vector<int>> matrix3 = {{3, 0, 8, 4}, {2, 4, 5, 7}, {9, 2, 6, 3}, {0, 3, 1, 0}};
vector<vector<char>> matrixchar = {{'1', '1', '1', '0'},
                                   {'1', '1', '0', '0'},
                                   {'1', '0', '1', '1'},
                                   {'0', '1', '0', '1'}};
vector<vector<char>> matrixchar2 = {{'1', '1', '1'}, {'0', '1', '0'}, {'1', '1', '1'}};
vector<vector<int>> matrixchar3 = {{17, 2, 4}, {16, 1, 5}, {14, 3, 4}};
vector<vector<int>> graphic1 = {{0, 1}, {0, 2}, {1, 2}, {1, 2}};

} // namespace MyOwnData
