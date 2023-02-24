#pragma once
#include <iostream>

#include <algorithm>

#include "BinSearchNode.h"
#include "DataObserver.h"

namespace KDebugger
{



class KDTree :public DataObserver
{
    enum class ERegionCrossState
    {
        out, part, in
    };

public:
    KDTree(DataGenerator* generator);
    void updateData()override;
    void getOneDRange(double l, double r);
    void getOneDRangeOri(double l, double r);

private:
    void reportSubTree(BinSearchNode* root, std::vector<KPt>& subnodes);

    // For 1D
    template<typename Iterator>
    BinSearchNode* buildFromSortedVec(BinSearchNode* parent,
        const std::vector<KPt>& vec, Iterator it, int cnt);

    // For 1D
    BinSearchNode* buildBinSearchTree(std::vector<KPt>& vec);

    // For 1D
    // 输入：树根, 两个数值, x, x', x<=x'
    // 输出：从树根出发分别通往x和x'的两条路径的分叉点v
    const BinSearchNode* FindSplitNode(const BinSearchNode* root,
        double leftnum, double rightnum);

    // For 1D
    // input: bin search tree, to-search-range [x,x']
    // output: vector<int> {numbers in to-search-range}
    void oneDRangeQuery(const BinSearchNode* root,
        double l, double r, std::vector<KPt>& res);

private:

    //2D
    BinSearchNode* buildKDTreeFromSortedVec(const std::vector<KPt>& pnts);

    void splitPnts(PntsSorted2D& pnts, PntsSorted2D& p1s, PntsSorted2D& p2s, bool direction);

    BinSearchNode* buildKDTree(PntsSorted2D pnts, int cur_depth);

    void searchKDTree(std::vector<PntsSorted2D>& res, BinSearchNode* root,
        const KRegion& cur_region, const KRegion& region);

    //search KD tree
    //input: root of kd-tree, to-search region
    //output: vector of point in region
    std::vector<PntsSorted2D> searchKDTreeFromRoot(BinSearchNode* root, const KRegion& region);



    ERegionCrossState regionCheck(const KRegion& regionl, const KRegion& regionr);

    KRegion getNewRegion(BinSearchNode* node, const KRegion& cur_region);

    void searchKDTree(std::vector<KPt>& res, BinSearchNode* root,
        const KRegion& cur_region, const KRegion& region);

private:
    std::vector<KPt> buf_;
    BinSearchNode* binsearch_1d_ = nullptr;
    BinSearchNode* binsearch_2d_ = nullptr;

};

}