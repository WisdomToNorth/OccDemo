#pragma once
#include "BinSearchNode.h"

namespace KDebugger
{
class KRangeTree
{
public:

    KRangeTree() {}

    BinSearchNode* buildRangeTree(const std::vector<KPt>& pnts);

    //search Range tree
    //input: root of  Range tree, to-search region
    //output: vector of point in region
    std::vector<KPt> searchRangeTreeFromRoot(BinSearchNode* root, const KRegion& region);
private:

};
}
