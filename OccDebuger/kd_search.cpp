
#include "kd_search.h"
#include "DataGenerator.h"
#include "Ktimer.h"

namespace KDebugger
{


KDTree::KDTree(DataGenerator* generator) :DataObserver(generator)
{
    updateData();
}

void KDTree::updateData()
{
    data_generator_->getPtData(buf_);
    if (binsearch_1d_)
    {
        delete binsearch_1d_;
        binsearch_1d_ = nullptr;
    }
    if (kd_2d_)
    {
        delete kd_2d_;
        kd_2d_ = nullptr;
    }
}

// For 1D
void KDTree::getOneDRangeOri(double l, double r)
{
    std::cout << "\n\n-----------1D Search------------" << std::endl;

    K_Timer timer;
    size_t cnt = 0;
    for (const auto& pt : buf_)
    {
        if (pt.x >= l && pt.x < r)
            cnt++;
    }

    std::cout << "\nOrigin: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << cnt << std::endl;
}

// For 1D
void KDTree::getOneDRange(double l, double r)
{
    std::cout << "\n\n-----------1D binnode search------------" << std::endl;

    K_Timer timer;
    if (!binsearch_1d_)
    {
        binsearch_1d_ = buildBinSearchTree(buf_);
        //std::cout << "\n ####tree start####\n";
        //printBinSearchTree(binsearch_1d_, true);
        //std::cout << "\n ####tree end####\n";
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> res;
    oneDRangeQuery(binsearch_1d_, l, r, res);

    std::cout << "\nGet Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << res.size() << std::endl;
    //printPntVec(res);
}

void KDTree::getTwoDRangeOri(const KRegion& r)
{
    std::cout << "\n\n-----------2D Search------------" << std::endl;
    std::vector<KPt> buf;
    K_Timer timer;
    size_t cnt = 0;
    for (const auto& pt : buf_)
    {
        if (r.ptInRegion(pt))
        {
            buf.emplace_back(pt);
            cnt++;
        }
    }
    std::cout << "\nOrigin: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << cnt << std::endl;
    //for (auto& pt : buf)pt.print();
}

void KDTree::getTwoDRangeKDTree(const KRegion& r)
{
    std::cout << "\n\n-----------2D Search KD------------" << std::endl;

    K_Timer timer;
    size_t cnt = 0;
    if (!kd_2d_)
    {
        kd_2d_ = buildKDTree(buf_);
        printBinSearchTree(kd_2d_);
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> buf = searchKDTreeFromRoot(kd_2d_, r);
    std::cout << "\nkd_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << buf.size() << std::endl;
    //for (auto& pt : buf)pt.print();
}

void KDTree::getSortedPnts(const std::vector<KPt>& pnts,
    PntsSorted2D& sorted_pnts)
{
    sorted_pnts.pnts_xsorted_ = pnts;
    sorted_pnts.pnts_ysorted_ = pnts;
    //sortx
    std::sort(sorted_pnts.pnts_xsorted_.begin(), sorted_pnts.pnts_xsorted_.end(),
        [](KPt v1, KPt v2) -> bool
        {
            return v1.x < v2.x;
        });
    // sorty
    std::sort(sorted_pnts.pnts_ysorted_.begin(), sorted_pnts.pnts_ysorted_.end(),
        [](KPt v1, KPt v2) -> bool
        {
            return v1.y < v2.y;
        });
}

BinSearchNode* KDTree::buildKDTree(const std::vector<KPt>& pnts)
{
    PntsSorted2D sorted_pnts;
    getSortedPnts(pnts, sorted_pnts);

    return buildKDTreeFromSortedPnts(sorted_pnts, 0);
}

KPt KDTree::splitPnts(const PntsSorted2D& pnts, PntsSorted2D& p1s,
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
                return v1.y < v2.y;
            });
        p2s.pnts_ysorted_ = p2s.pnts_xsorted_;
        std::sort(p2s.pnts_ysorted_.begin(), p2s.pnts_ysorted_.end(),
            [](KPt v1, KPt v2) -> bool
            {
                return v1.y < v2.y;
            });
        midpt = *(pnts.pnts_xsorted_.begin() + mid);
        /* double mid_x = (pnts.pnts_xsorted_.begin() + mid)->x;
         for (const KPt& pnt : pnts.pnts_ysorted_)
         {
             if (pnt.x < mid_x)
             {
                 p1s.pnts_ysorted_.emplace_back(pnt);
             }
             else
             {
                 p2s.pnts_ysorted_.emplace_back(pnt);
             }
         }*/
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
        //double mid_y = (pnts.pnts_ysorted_.begin() + mid-1)->y;
        ////需要用前一组的最后一个元素，判定时，取小于等于，值
        //for (const KPt& pnt : pnts.pnts_xsorted_)
        //{
        //    if (pnt.y < mid_y)
        //    {
        //        p1s.pnts_xsorted_.emplace_back(pnt);
        //    }
        //    else
        //    {
        //        p2s.pnts_xsorted_.emplace_back(pnt);
        //    }
        //}
        p1s.size();
        p2s.size();
    }
    return midpt;
}

BinSearchNode* KDTree::buildKDTreeFromSortedPnts(PntsSorted2D pnts,
    int cur_depth)
{
    size_t n = pnts.size();
    //std::cout << "split n: " << n << std::endl;
    if (n == 0)
        return nullptr;
    else if (n == 1)
        return new BinSearchNode(pnts.getOnlyPnt());
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

        node->left_ = buildKDTreeFromSortedPnts(p1s, cur_depth + 1);
        node->right_ = buildKDTreeFromSortedPnts(p2s, cur_depth + 1);
        return node;
    }
}

//search KD tree
//input: root of kd-tree, to-search region
//output: vector of point in region
std::vector<KPt> KDTree::searchKDTreeFromRoot(BinSearchNode* root,
    const KRegion& region)
{
    std::vector<KPt> res;

    KRegion init_region(KPt(INT8_MIN, INT8_MIN), KPt(INT8_MAX, INT8_MAX));
    searchKDTree(res, root, init_region, region);
    return res;
}

KRegion KDTree::getNewRegion(const KRegion& cur_region, const BinSearchNode* node, bool f_or_s)
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

void KDTree::searchKDTree(std::vector<KPt>& res, BinSearchNode* root,
    const KRegion& cur_region, const KRegion& region)
{
    if (root->isLeaf())
    {
        if (root->BinSearchNodeInRegion(region))res.emplace_back(root->pnt_);
    }
    else
    {
        KRegion left_region = getNewRegion(cur_region, root, true);
        auto check_res_l = regionCheck(left_region, region);
        if (check_res_l == ERegionCrossState::in)
        {
            reportSubTree(root->left_, res);
        }
        else if (check_res_l == ERegionCrossState::part)
        {
            searchKDTree(res, root->left_, left_region, region);
        }

        KRegion right_region = getNewRegion(cur_region, root, false);
        auto check_res_r = regionCheck(right_region, region);
        if (check_res_r == ERegionCrossState::in)
        {
            reportSubTree(root->right_, res);
        }
        else if (check_res_r == ERegionCrossState::part)
        {
            searchKDTree(res, root->right_, right_region, region);
        }
    }
}
}