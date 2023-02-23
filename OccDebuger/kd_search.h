#pragma once
#include <iostream>

#include <algorithm>

#include "BinSearchNode.h"
#include "DataObserver.h"

namespace KDebugger
{



class KDTree :public DataObserver
{
public:
    KDTree(DataGenerator* generator);
    void updateData()override;
    void getOneDRange(double l, double r);

private:

    void reportSubTree(BinSearchNode* root, std::vector<KPt>& subnodes);

    template<typename Iterator>
    BinSearchNode* buildFromSortedVec(BinSearchNode* parent,
        const std::vector<KPt>& vec, Iterator it, int cnt);

    BinSearchNode* buildBinSearchTree(std::vector<KPt>& vec);



    // 输入：树根, 两个数值, x, x', x<=x'
    // 输出：从树根出发分别通往x和x'的两条路径的分叉点v
    BinSearchNode* FindSplitNode(BinSearchNode* root,
        int leftnum, int rightnum);

    // input: bin search tree, to-search-range [x,x']
    // output: vector<int> {numbers in to-search-range}
    void oneDRangeQuery(BinSearchNode* root,
        double l, double r, std::vector<KPt>& res);
private:
    std::vector<KPt> buf_;

};

//BinSearchNode* buildKDTree(PntsSorted2D pnts, int cur_depth);
//BinSearchNode* buildKDTreeFromSortedVec(const std::vector<KPt>& pnts)
//{
//    PntsSorted2D p_pnts;
//    p_pnts.pnts_xsorted_ = pnts;
//    p_pnts.pnts_ysorted_ = pnts;
//    //sortx
//    std::sort(p_pnts.pnts_xsorted_.begin(), p_pnts.pnts_xsorted_.end());
//    // sorty
//    std::sort(p_pnts.pnts_ysorted_.begin(), p_pnts.pnts_ysorted_.end(),
//        [](KPt v1, KPt v2) -> bool
//        {
//            return v1.y < v2.y;
//        });
//    return buildKDTree(p_pnts, 0);
//}
//
//void splitPnts(PntsSorted2D& pnts, PntsSorted2D& p1s, PntsSorted2D& p2s, bool direction)
//{
//    int n = pnts.pnts_xsorted_.size();
//    int mid = n / 2;
//    if (direction)//vertical by mid x
//    {
//        //put into p1s
//        p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(),
//            pnts.pnts_xsorted_.begin() + mid); // 0 to mid
//        //put into p2s
//        p2s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin() + mid,
//            pnts.pnts_xsorted_.end()); // mid to last
//
//        double mid_x = (pnts.pnts_xsorted_.begin() + mid)->x;
//        for (const KPt& pnt : pnts.pnts_ysorted_)
//        {
//            if (pnt.x <= mid_x)
//            {
//                p1s.pnts_ysorted_.emplace_back(pnt);
//            }
//            else
//            {
//                p2s.pnts_ysorted_.emplace_back(pnt);
//            }
//        }
//    }
//    else//horizon by mid y
//    {
//        //put into p1s
//        p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
//            pnts.pnts_ysorted_.begin() + mid); // 0 to mid
//        //put into p2s
//        p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
//            pnts.pnts_ysorted_.end()); // mid to last
//
//        double mid_y = (pnts.pnts_ysorted_.begin() + mid)->y;
//        for (const KPt& pnt : pnts.pnts_xsorted_)
//        {
//            if (pnt.y <= mid_y)
//            {
//                p1s.pnts_xsorted_.emplace_back(pnt);
//            }
//            else
//            {
//                p2s.pnts_xsorted_.emplace_back(pnt);
//            }
//        }
//    }
//}
//
//BinSearchNode* buildKDTree(PntsSorted2D pnts, int cur_depth)
//{
//
//    int n = pnts.pnts_xsorted_.size();
//    if (n == 0)
//        return nullptr;
//    else if (n == 1)
//        return new BinSearchNode(pnts.getOnlyPnt());
//    else
//    {
//        PntsSorted2D p1s;
//        PntsSorted2D p2s;
//        int mid = n / 2;
//        BinSearchNode* node = new BinSearchNode();
//        if (cur_depth % 2) //vert
//        {
//            splitPnts(pnts, p1s, p2s, 1);
//            node->direction_ = 1;
//        }
//        else//horizon
//        {
//            splitPnts(pnts, p1s, p2s, 0);
//
//            node->direction_ = 0;
//        }
//
//        node->left_ = buildKDTree(p1s, cur_depth + 1);
//        node->right_ = buildKDTree(p2s, cur_depth + 1);
//    }
//}
////declare
//void searchKDTree(std::vector<PntsSorted2D>& res, BinSearchNode* root,
//    const KRegion& cur_region, const KRegion& region);
//
////search KD tree
////input: root of kd-tree, to-search region
////output: vector of point in region
//std::vector<PntsSorted2D> searchKDTreeFromRoot(BinSearchNode* root, const KRegion& region)
//{
//    std::vector<PntsSorted2D> res;
//
//    KRegion init_region(KPt(INT8_MIN, INT8_MIN), KPt(INT8_MAX, INT8_MAX));
//    searchKDTree(res, root, init_region, region);
//    return res;
//}
//
//enum class ERegionCrossState
//{
//    out, part, in
//};
//
//ERegionCrossState regionCheck(const KRegion& regionl, const KRegion& regionr)
//{
//
//    //todo
//    return ERegionCrossState::part;
//}
//
//KRegion getNewRegion(BinSearchNode* node, const KRegion& cur_region)
//{
//
//}
//
//void searchKDTree(std::vector<KPt>& res, BinSearchNode* root,
//    const KRegion& cur_region, const KRegion& region)
//{
//    if (root->isLeaf())
//    {
//        if (root->BinSearchNodeInRegion(region))res.emplace_back(root->pnt_);
//    }
//    else
//    {
//        KRegion left_region = getNewRegion(root->left_, cur_region);
//        auto check_res_l = regionCheck(left_region, region);
//        if (check_res_l == ERegionCrossState::in)
//        {
//            reportSubTree(root->left_, res);
//        }
//        else if (check_res_l == ERegionCrossState::part)
//        {
//            searchKDTree(res, root->left_, left_region, region);
//        }
//
//        KRegion right_region = getNewRegion(root->right_, cur_region);
//        auto check_res_r = regionCheck(right_region, region);
//        if (check_res_r == ERegionCrossState::in)
//        {
//            reportSubTree(root->right_, res);
//        }
//        else if (check_res_r == ERegionCrossState::part)
//        {
//            searchKDTree(res, root->right_, right_region, region);
//        }
//    }
//}
}