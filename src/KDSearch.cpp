#include "KDSearch.h"

#include <vector>
#include <iterator>
#include <forward_list>
#include <algorithm>

#include "multisort.h"


namespace KDebugger
{
KDSearch::KDSearch() {}

BinSearchNode* KDSearch::buildTwoDSearch(const std::vector<KPt>& pnts)
{
    PntsSorted2D sorted_pnts(pnts);
    return buildTwoDSearchFromSortedPnts(sorted_pnts, 0);
}

KPt KDSearch::splitPnts_OLD(PntsSorted2D& pnts, PntsSorted2D& p1s,
    PntsSorted2D& p2s, bool direction)
{
    int n = pnts.size();
    int mid = n / 2;
    KPt midpt;
    if (direction)//vertical by mid x
    {
        //put into p1s
        p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(),
            pnts.pnts_xsorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin() + mid,
            pnts.pnts_xsorted_.end()); // mid to last
        midpt = *(pnts.pnts_xsorted_.begin() + mid);

        ////直接拷贝并且重排，不好的方法
        p1s.pnts_ysorted_ = p1s.pnts_xsorted_;
        Sort_YS(p1s.pnts_ysorted_.begin(), p1s.pnts_ysorted_.end());

        p2s.pnts_ysorted_ = p2s.pnts_xsorted_;
        Sort_YS(p2s.pnts_ysorted_.begin(), p2s.pnts_ysorted_.end());

        //数出来的肯定是更多的。
        //double mid_x = (pnts.pnts_xsorted_.begin() + mid)->x;
        //auto it = std::stable_partition(pnts.pnts_ysorted_.begin(),
        //    pnts.pnts_ysorted_.end(), [&](KPt& pt)
        //    {
        //        return pt.x <= mid_x;
        //    });
        ////需要在此集合中先选严格小于mid_x的值，再选等于mid_x时从大到小排y值
        //auto it_aux = std::stable_partition(pnts.pnts_ysorted_.begin(),
        //    it, [&](KPt& pt)
        //    {
        //        return pt.x < mid_x;
        //    });


        //int gap = int(p1s.pnts_xsorted_.size()) -
        //    int(std::distance(pnts.pnts_ysorted_.begin(), it_aux));
        //std::advance(it_aux, gap);
        //p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(), it_aux);
        //p2s.pnts_ysorted_.assign(it_aux, pnts.pnts_ysorted_.end());
        ////如果等于mid的值的y小，则不应该在此处，此时整体y大致有序，
        ////因此，使用冒泡排序，可以在较短时间还原y顺序

        ////此方案理论上可以大幅度减少无用排序。下面的数组在排序前基本是已经有序的
        ////但是使用冒泡排序提前终止的话，速度还是比std::sort慢得多
        //Sort_YL(p1s.pnts_ysorted_.begin(), p1s.pnts_ysorted_.end());
        //Sort_YL(p2s.pnts_ysorted_.begin(), p2s.pnts_ysorted_.end());

    }
    else//horizon by mid y
    {
        //put into p1s
        p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
            pnts.pnts_ysorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
            pnts.pnts_ysorted_.end()); // mid to last
        midpt = *(pnts.pnts_ysorted_.begin() + mid - 1);

        ////直接拷贝并且重排，不好的方法
        p1s.pnts_xsorted_ = p1s.pnts_ysorted_;
        Sort_XS(p1s.pnts_xsorted_.begin(), p1s.pnts_xsorted_.end());
        p2s.pnts_xsorted_ = p2s.pnts_ysorted_;
        Sort_XS(p2s.pnts_xsorted_.begin(), p2s.pnts_xsorted_.end());

        //double mid_y = (pnts.pnts_ysorted_.begin() + mid)->y;
        //auto it = std::stable_partition(pnts.pnts_xsorted_.begin(),
        //    pnts.pnts_xsorted_.end(), [&](KPt& pt)
        //    {
        //        return pt.y >= mid_y;
        //    });
        ////需要在此集合中先选严格小于mid_y的值，再选等于mid_y时从大到小排y值
        //auto it_aux = std::stable_partition(pnts.pnts_xsorted_.begin(), it,
        //    [&](KPt& pt)
        //    {
        //        return pt.y > mid_y;
        //    });

        //int gap = int(p1s.pnts_ysorted_.size()) -
        //    int(std::distance(pnts.pnts_xsorted_.begin(), it_aux));
        //std::advance(it_aux, gap);
        //p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(), it_aux);
        //p2s.pnts_xsorted_.assign(it_aux, pnts.pnts_xsorted_.end());
        ////如果等于mid的值的y小，则不应该在此处，此时整体y大致有序，


        //Sort_XS(p1s.pnts_xsorted_.begin(), p1s.pnts_xsorted_.end());
        //Sort_XS(p2s.pnts_xsorted_.begin(), p2s.pnts_xsorted_.end());

    }
    return midpt;
}

KPt KDSearch::splitPnts(PntsSorted2D& pnts, PntsSorted2D& p1s,
    PntsSorted2D& p2s, bool direction)
{
    int n = pnts.size();
    int mid = n / 2;
    KPt midpt;

    if (direction)//vertical by mid x
    {
        //put into p1s
        p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(),
            pnts.pnts_xsorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin() + mid,
            pnts.pnts_xsorted_.end()); // mid to last
        midpt = *(pnts.pnts_xsorted_.begin() + mid);

        ////直接拷贝并且重排，不好的方法
        p1s.pnts_ysorted_ = p1s.pnts_xsorted_;
        Sort_YS(p1s.pnts_ysorted_.begin(), p1s.pnts_ysorted_.end());

        p2s.pnts_ysorted_ = p2s.pnts_xsorted_;
        Sort_YS(p2s.pnts_ysorted_.begin(), p2s.pnts_ysorted_.end());


    }
    else//horizon by mid y
    {
        //put into p1s
        p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
            pnts.pnts_ysorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
            pnts.pnts_ysorted_.end()); // mid to last
        midpt = *(pnts.pnts_ysorted_.begin() + mid - 1);

        ////直接拷贝并且重排，不好的方法
        p1s.pnts_xsorted_ = p1s.pnts_ysorted_;
        Sort_XS(p1s.pnts_xsorted_.begin(), p1s.pnts_xsorted_.end());
        p2s.pnts_xsorted_ = p2s.pnts_ysorted_;
        Sort_XS(p2s.pnts_xsorted_.begin(), p2s.pnts_xsorted_.end());


    }
    return midpt;
}
BinSearchNode* KDSearch::buildTwoDSearchFromSortedPnts(PntsSorted2D& pnts,
    int cur_depth)
{
    size_t n = pnts.size();
    if (n == 0)
        return nullptr;
    else if (n == 1)
        return new BinSearchNode(pnts.getOnlyPnt(), false);
    else
    {
        PntsSorted2D p1s;
        PntsSorted2D p2s;
        BinSearchNode* node = new BinSearchNode();
        if (cur_depth % 2) //vert
        {
            node->pnt_ = splitPnts(pnts, p1s, p2s, 1);
            node->direction_ = 1;
        }
        else//horizon
        {
            node->pnt_ = splitPnts(pnts, p1s, p2s, 0);

            node->direction_ = 0;
        }

        node->left_ = buildTwoDSearchFromSortedPnts(p1s, cur_depth + 1);
        node->right_ = buildTwoDSearchFromSortedPnts(p2s, cur_depth + 1);
        return node;
    }
}

//search KD tree
//input: root of kd-tree, to-search region
//output: vector of point in region
std::vector<KPt> KDSearch::searchTwoDSearchFromRoot(BinSearchNode* root,
    const KRegion& region)
{
    std::vector<KPt> res;

    KRegion init_region(KPt(INT8_MIN, INT8_MIN), KPt(INT8_MAX, INT8_MAX));
    searchTwoDSearch(res, root, init_region, region);
    return res;
}

KRegion KDSearch::getNewRegion(const KRegion& cur_region,
    const BinSearchNode* node, bool f_or_s)
{

    if (f_or_s)//true, first
    {
        if (node->direction_)//left
            return KRegion(cur_region.lb_, KPt(node->pnt_.x, cur_region.ru_.y));
        else//bottom
            return KRegion(cur_region.lb_, KPt(cur_region.ru_.x, node->pnt_.y));
    }
    else
    {
        if (node->direction_)//right
            return KRegion(KPt(node->pnt_.x, cur_region.lb_.y), cur_region.ru_);
        else //up
            return KRegion(KPt(cur_region.lb_.x, node->pnt_.y), cur_region.ru_);
    }
}

void KDSearch::searchTwoDSearch(std::vector<KPt>& res, BinSearchNode* root,
    const KRegion& cur_region, const KRegion& region)
{
    if (root->isLeaf())
    {
        if (root->belongToRegion(region))res.emplace_back(root->pnt_);
    }
    else
    {
        KRegion left_region = getNewRegion(cur_region, root, true);
        auto check_res_l = regionCheck(left_region, region);
        if (check_res_l == ERegionCrossState::in)
        {
            root->left_->reportSubTree(res);
        }
        else if (check_res_l == ERegionCrossState::part)
        {
            searchTwoDSearch(res, root->left_, left_region, region);
        }

        KRegion right_region = getNewRegion(cur_region, root, false);
        auto check_res_r = regionCheck(right_region, region);
        if (check_res_r == ERegionCrossState::in)
        {
            root->right_->reportSubTree(res);
        }
        else if (check_res_r == ERegionCrossState::part)
        {
            searchTwoDSearch(res, root->right_, right_region, region);
        }
    }
}
}
