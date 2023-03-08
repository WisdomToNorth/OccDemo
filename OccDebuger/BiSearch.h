#pragma once

#include <vector>
#include "BinSearchNode.h"

namespace KDebugger
{
class BiSearch
{
public:
    BiSearch();

    // For 1D
    BinSearchNode* buildBinSearchTree(std::vector<KPt>& vec);
    // For 1D
// input: bin search tree, to-search-range [x,x']
// output: vector<int> {numbers in to-search-range}
    void oneDRangeQuery(const BinSearchNode* root,
        double l, double r, std::vector<KPt>& res);
private:


    // For 1D
    template<typename Iterator>
    BinSearchNode* buildFromSortedVec(BinSearchNode* parent,
        const std::vector<KPt>& vec, Iterator it, int cnt);


    // For 1D
    // 输入：树根, 两个数值, x, x', x<=x'
    // 输出：从树根出发分别通往x和x'的两条路径的分叉点v
    const BinSearchNode* FindSplitNode(const BinSearchNode* root,
        double leftnum, double rightnum);


};

}