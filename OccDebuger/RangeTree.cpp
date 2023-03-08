#include "RangeTree.h"

namespace KDebugger
{


BinSearchNode* KRangeTree::buildRangeTree(const std::vector<KPt>& pnts)
{
    return new BinSearchNode();
}

std::vector<KPt> KRangeTree::searchRangeTreeFromRoot
(BinSearchNode* root, const KRegion& region)
{

    std::vector<KPt> res;
    return res;
}

}