#include "KDSearch.h"

#include "multisort.h"

namespace KDebugger
{
KDSearch::KDSearch() {}


void KDSearch::getSortedPnts(const std::vector<KPt>& pnts,
    PntsSorted2D& sorted_pnts)
{
    sorted_pnts.pnts_xsorted_ = pnts;
    sorted_pnts.pnts_ysorted_ = pnts;
    ////sortx
    //std::sort(sorted_pnts.pnts_xsorted_.begin(), sorted_pnts.pnts_xsorted_.end(),
    //    [](KPt v1, KPt v2) -> bool
    //    {
    //        return v1.x < v2.x;
    //    });
    //// sorty
    //std::sort(sorted_pnts.pnts_ysorted_.begin(), sorted_pnts.pnts_ysorted_.end(),
    //    [](KPt v1, KPt v2) -> bool
    //    {
    //        return v1.y > v2.y;
    //    });
    Sort_XS(sorted_pnts.pnts_xsorted_.begin(), sorted_pnts.pnts_xsorted_.end());
    Sort_YL(sorted_pnts.pnts_ysorted_.begin(), sorted_pnts.pnts_ysorted_.end());
}

BinSearchNode* KDSearch::buildTwoDSearch(const std::vector<KPt>& pnts)
{
    PntsSorted2D sorted_pnts;
    getSortedPnts(pnts, sorted_pnts);

    return buildTwoDSearchFromSortedPnts(sorted_pnts, 0);
}

KPt KDSearch::splitPnts(const PntsSorted2D& pnts, PntsSorted2D& p1s,
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

        //todo: need not sort here
        p1s.pnts_ysorted_ = p1s.pnts_xsorted_;
        std::sort(p1s.pnts_ysorted_.begin(), p1s.pnts_ysorted_.end(),
            [](KPt v1, KPt v2) -> bool
            {
                return v1.y > v2.y;
            });
        p2s.pnts_ysorted_ = p2s.pnts_xsorted_;
        std::sort(p2s.pnts_ysorted_.begin(), p2s.pnts_ysorted_.end(),
            [](KPt v1, KPt v2) -> bool
            {
                return v1.y > v2.y;
            });
        midpt = *(pnts.pnts_xsorted_.begin() + mid);

        p1s.size();
        p2s.size();
    }
    else//horizon by mid y
    {
        //put into p1s
        p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
            pnts.pnts_ysorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
            pnts.pnts_ysorted_.end()); // mid to last

        //todo: need not sort here
        p1s.pnts_xsorted_ = p1s.pnts_ysorted_;
        std::sort(p1s.pnts_xsorted_.begin(), p1s.pnts_xsorted_.end(),
            [](KPt v1, KPt v2) -> bool
            {
                return v1.x < v2.x;
            });
        p2s.pnts_xsorted_ = p2s.pnts_ysorted_;
        std::sort(p2s.pnts_xsorted_.begin(), p2s.pnts_xsorted_.end(),
            [](KPt v1, KPt v2) -> bool
            {
                return v1.x < v2.x;
            });
        midpt = *(pnts.pnts_ysorted_.begin() + mid - 1);

        p1s.size();
        p2s.size();
    }
    return midpt;
}

BinSearchNode* KDSearch::buildTwoDSearchFromSortedPnts(PntsSorted2D pnts,
    int cur_depth)
{
    size_t n = pnts.size();
    //std::cout << "split n: " << n << std::endl;
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

KRegion KDSearch::getNewRegion(const KRegion& cur_region, const BinSearchNode* node, bool f_or_s)
{
    if (f_or_s)//true, first
    {
        if (node->direction_)//left
            return KRegion(cur_region.lb_, KPt(node->pnt_.x, cur_region.ru_.y));
        else//up
            return KRegion(KPt(cur_region.lb_.x, node->pnt_.y), cur_region.ru_);

    }
    else
    {
        if (node->direction_)//right
            return KRegion(KPt(node->pnt_.x, cur_region.lb_.y), cur_region.ru_);
        else //down
            return KRegion(cur_region.lb_, KPt(cur_region.ru_.x, node->pnt_.y));
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
            // reportSubTree(root->left_, res);
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
            //reportSubTree(root->right_, res);
            root->right_->reportSubTree(res);
        }
        else if (check_res_r == ERegionCrossState::part)
        {
            searchTwoDSearch(res, root->right_, right_region, region);
        }
    }
}
}
