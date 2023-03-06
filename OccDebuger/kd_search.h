#pragma once
#include <iostream>

#include <algorithm>

#include "BinSearchNode.h"
#include "DataObserver.h"
#include "KRegion.h"

namespace KDebugger
{



class KDTree :public DataObserver
{

public:
    KDTree(DataGenerator* generator);
    void updateData()override;
    void getOneDRange(double l, double r);
    void getOneDRangeOri(double l, double r);
    void getTwoDRangeKDTree(const KRegion& r);
    void getTwoDRangeOri(const KRegion& r);

private:


private:

    //2D
    BinSearchNode* buildKDTree(const std::vector<KPt>& pnts);
    void getSortedPnts(const std::vector<KPt>& pnts,
        PntsSorted2D& p_pnts);


    KPt splitPnts(const PntsSorted2D& pnts, PntsSorted2D& p1s, PntsSorted2D& p2s, bool direction);

    BinSearchNode* buildKDTreeFromSortedPnts(PntsSorted2D pnts, int cur_depth);

    //search KD tree
    //input: root of kd-tree, to-search region
    //output: vector of point in region
    std::vector<KPt> searchKDTreeFromRoot(BinSearchNode* root, const KRegion& region);



    //true for first, false for second
    KRegion getNewRegion(const KRegion& cur_region, const BinSearchNode* node,
        bool f_or_s);

    void searchKDTree(std::vector<KPt>& res, BinSearchNode* root,
        const KRegion& check_in_this_region, const KRegion& region);

private:
    std::vector<KPt> buf_;
    BinSearchNode* binsearch_1d_ = nullptr;
    BinSearchNode* kd_2d_ = nullptr;

};

}