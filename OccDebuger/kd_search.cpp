
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

void KDTree::reportSubTree(BinSearchNode* root,
    std::vector<KPt>& subnodes)//1D
{
    //std::cout << "\n%%%%report subtree: \n";
    //printBinSearchTree(root);
    //std::cout << "\n%%%%report subtree end \n";
    if (root->isLeaf())
    {
        subnodes.emplace_back(root->pnt_);
    }
    if (root->left_)
        reportSubTree(root->left_, subnodes);
    if (root->right_)
        reportSubTree(root->right_, subnodes);
}

// For 1D
template<typename Iterator>
BinSearchNode* KDTree::buildFromSortedVec(BinSearchNode* parent,
    const std::vector<KPt>& vec, Iterator it, int cnt)//1D
{

    assert(cnt > 0);
    if (cnt == 1)
    {
        parent->pnt_ = *it;
        return parent;
    }
    //计数的时候添加哨兵会更容易
    // -,1,2,3
    // -,1,2,3,4


    int cnt_P1 = cnt + 1;
    int leftcnt = cnt_P1 / 2;
    int rightcnt = cnt - leftcnt;
    auto mid_it = it;
    std::advance(mid_it, leftcnt - 1);

    parent->pnt_ = *mid_it;
    parent->left_ = buildFromSortedVec(new BinSearchNode(), vec, it, leftcnt);
    parent->right_ = buildFromSortedVec(new BinSearchNode(), vec, mid_it + 1, rightcnt);
    return parent;
}

// For 1D
BinSearchNode* KDTree::buildBinSearchTree(std::vector<KPt>& vec)//1D
{
    std::sort(vec.begin(), vec.end(),
        [](KPt& pt1, KPt& pt2)
        {
            return pt1.x < pt2.x;
        });
    //std::cout << "\nAfter sort:" << std::endl;
    //printPntVec(vec);

    int n = vec.size();
    BinSearchNode* root = new BinSearchNode();
    return buildFromSortedVec(root, vec, vec.begin(), vec.size());
}

// For 1D
// 输入：树根, 两个数值, x, x', x<=x'
// 输出：从树根出发分别通往x和x'的两条路径的分叉点v
const BinSearchNode* KDTree::FindSplitNode(const BinSearchNode* root,
    double leftnum, double rightnum)//1D
{
    const BinSearchNode* v = root;
    while (!v->isLeaf() && (rightnum <= v->pnt_.x || leftnum > v->pnt_.x))
    {
        if (rightnum < v->pnt_.x)
            v = v->left_;
        else
            v = v->right_;
    }
    //std::cout << "\n####spliter of [" << leftnum << ", " << rightnum << "):\n";
    //printBinSearchTree(v, true);
    //std::cout << std::endl;
    return v;
}

// For 1D
// input: bin search tree, to-search-range [x,x']
// output: vector<int> {numbers in to-search-range}
void KDTree::oneDRangeQuery(const BinSearchNode* root,
    double l, double r, std::vector<KPt>& res)
{
    const BinSearchNode* v_split = FindSplitNode(root, l, r);
    if (v_split->isLeaf())
    {
        if (v_split->belongToRangeInX(l, r))
            res.emplace_back(v_split->pnt_);
    }
    else
    {
        // find way to left
        const BinSearchNode* l_split = v_split->left_;
        while (!l_split->isLeaf())
        {
            if (l_split->pnt_.x >= l)
            { // report right tree
                reportSubTree(l_split->right_, res);
                l_split = l_split->left_;
            }
            else
            {
                l_split = l_split->right_;
            }
        }
        if (l_split->belongToRangeInX(l, r))
            res.emplace_back(l_split->pnt_);

        // find way to right;
        const BinSearchNode* r_split = v_split->right_;
        while (!r_split->isLeaf())
        {
            if (r_split->pnt_.x < r)
            { // report right tree
                reportSubTree(r_split->left_, res);
                r_split = r_split->right_;
            }
            else
            {
                r_split = r_split->left_;
            }
        }
        if (r_split->belongToRangeInX(l, r))
            res.emplace_back(r_split->pnt_);

    }
}

BinSearchNode* KDTree::buildKDTreeFromSortedVec(const std::vector<KPt>& pnts)
{
    PntsSorted2D p_pnts;
    p_pnts.pnts_xsorted_ = pnts;
    p_pnts.pnts_ysorted_ = pnts;
    //sortx
    std::sort(p_pnts.pnts_xsorted_.begin(), p_pnts.pnts_xsorted_.end(),
        [](KPt v1, KPt v2) -> bool
        {
            return v1.x < v2.x;
        });
    // sorty
    std::sort(p_pnts.pnts_ysorted_.begin(), p_pnts.pnts_ysorted_.end(),
        [](KPt v1, KPt v2) -> bool
        {
            return v1.y < v2.y;
        });
    return buildKDTree(p_pnts, 0);
}

void KDTree::splitPnts(PntsSorted2D& pnts, PntsSorted2D& p1s, PntsSorted2D& p2s, bool direction)
{
    int n = pnts.pnts_xsorted_.size();
    int mid = n / 2;
    if (direction)//vertical by mid x
    {
        //put into p1s
        p1s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin(),
            pnts.pnts_xsorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_xsorted_.assign(pnts.pnts_xsorted_.begin() + mid,
            pnts.pnts_xsorted_.end()); // mid to last

        double mid_x = (pnts.pnts_xsorted_.begin() + mid)->x;
        for (const KPt& pnt : pnts.pnts_ysorted_)
        {
            if (pnt.x <= mid_x)
            {
                p1s.pnts_ysorted_.emplace_back(pnt);
            }
            else
            {
                p2s.pnts_ysorted_.emplace_back(pnt);
            }
        }
    }
    else//horizon by mid y
    {
        //put into p1s
        p1s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin(),
            pnts.pnts_ysorted_.begin() + mid); // 0 to mid
        //put into p2s
        p2s.pnts_ysorted_.assign(pnts.pnts_ysorted_.begin() + mid,
            pnts.pnts_ysorted_.end()); // mid to last

        double mid_y = (pnts.pnts_ysorted_.begin() + mid)->y;
        for (const KPt& pnt : pnts.pnts_xsorted_)
        {
            if (pnt.y <= mid_y)
            {
                p1s.pnts_xsorted_.emplace_back(pnt);
            }
            else
            {
                p2s.pnts_xsorted_.emplace_back(pnt);
            }
        }
    }
}

BinSearchNode* KDTree::buildKDTree(PntsSorted2D pnts, int cur_depth)
{

    int n = pnts.pnts_xsorted_.size();
    if (n == 0)
        return nullptr;
    else if (n == 1)
        return new BinSearchNode(pnts.getOnlyPnt());
    else
    {
        PntsSorted2D p1s;
        PntsSorted2D p2s;
        int mid = n / 2;
        BinSearchNode* node = new BinSearchNode();
        if (cur_depth % 2) //vert
        {
            splitPnts(pnts, p1s, p2s, 1);
            node->direction_ = 1;
        }
        else//horizon
        {
            splitPnts(pnts, p1s, p2s, 0);

            node->direction_ = 0;
        }

        node->left_ = buildKDTree(p1s, cur_depth + 1);
        node->right_ = buildKDTree(p2s, cur_depth + 1);
        return node;
    }
}

//search KD tree
//input: root of kd-tree, to-search region
//output: vector of point in region
std::vector<PntsSorted2D> KDTree::searchKDTreeFromRoot(BinSearchNode* root, const KRegion& region)
{
    std::vector<PntsSorted2D> res;

    KRegion init_region(KPt(INT8_MIN, INT8_MIN), KPt(INT8_MAX, INT8_MAX));
    searchKDTree(res, root, init_region, region);
    return res;
}

KDTree::ERegionCrossState KDTree::regionCheck(const KRegion& regionl, const KRegion& regionr)
{

    //todo
    return KDTree::ERegionCrossState::part;
}

KRegion KDTree::getNewRegion(BinSearchNode* node, const KRegion& cur_region)
{
    return KRegion(KPt(), KPt());
}
void KDTree::searchKDTree(std::vector<PntsSorted2D>& res, BinSearchNode* root,
    const KRegion& cur_region, const KRegion& region)
{

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
        KRegion left_region = getNewRegion(root->left_, cur_region);
        auto check_res_l = regionCheck(left_region, region);
        if (check_res_l == ERegionCrossState::in)
        {
            reportSubTree(root->left_, res);
        }
        else if (check_res_l == ERegionCrossState::part)
        {
            searchKDTree(res, root->left_, left_region, region);
        }

        KRegion right_region = getNewRegion(root->right_, cur_region);
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