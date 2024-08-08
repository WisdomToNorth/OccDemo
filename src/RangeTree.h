#pragma once

#include <vector>

#include "BinSearchNode.h"
#include "BiSearch.h"

namespace KDebugger
{

class KRangeTree
{
public:
    KRangeTree()
    {}

    static BinSearchNode *buildRangeTree(std::vector<KPt> &pnts);

    // For 1D
    // 输入：树根, 两个数值, x, x', x<=x'
    // 输出：从树根出发分别通往x和x'的两条路径的分叉点v
    static const BinSearchNode *FindSplitNodeInX(const BinSearchNode *root,
                                                 double leftnum, double rightnum);

    // search Range tree
    // input: root of Range tree, to-search region
    // output: vector of point in region
    static void searchRangeTreeFromRoot(BinSearchNode *root,
                                        const KRegion &region, std::vector<KPt> &res);

private:
    // For 1D
    template <typename Iterator>
    static BinSearchNode *buildRangeTreeFromSortedVec(BinSearchNode *parent,
                                                      const std::vector<KPt> &vec, Iterator it, size_t cnt);
};
} // namespace KDebugger
