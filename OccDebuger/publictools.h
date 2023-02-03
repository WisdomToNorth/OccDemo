#pragma once
#include <iostream>
#include <vector>
using namespace std;
void printVec2D(vector<vector<int>> vec)
{
    std::cout << '[';
    for (const auto& a : vec)
    {
        std::cout << '[';
        for (auto b = a.cbegin(); b != a.cend(); ++b)
        {
            std::cout << *b;
            if (b != --a.cend())std::cout << ", ";
        }
        std::cout << ']';
    }
    std::cout << "]\n";
}
