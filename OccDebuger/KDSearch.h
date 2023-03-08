#pragma once

#include "BinSearchNode.h"
#include "KPnt.h"

namespace KDebugger
{
class KDSearch
{
public:
    KDSearch();
    //2D
    BinSearchNode* buildKDTree(const std::vector<KPt>& pnts);

    //search KD tree
    //input: root of kd-tree, to-search region
    //output: vector of point in region
    std::vector<KPt> searchKDTreeFromRoot(BinSearchNode* root, const KRegion& region);

private:


    void getSortedPnts(const std::vector<KPt>& pnts,
        PntsSorted2D& p_pnts);

    KPt splitPnts(const PntsSorted2D& pnts, PntsSorted2D& p1s, PntsSorted2D& p2s, bool direction);

    BinSearchNode* buildKDTreeFromSortedPnts(PntsSorted2D pnts, int cur_depth);

    //true for first, false for second
    KRegion getNewRegion(const KRegion& cur_region, const BinSearchNode* node,
        bool f_or_s);

    void searchKDTree(std::vector<KPt>& res, BinSearchNode* root,
        const KRegion& check_in_this_region, const KRegion& region);

    //private:
    //    BinSearchNode* kd_2d_ = nullptr;
    //

};

}
