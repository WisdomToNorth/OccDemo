
#include "2d_search.h"
#include "DataGenerator.h"
#include "KTimer.h"
#include "BiSearch.h"

#include "RangeTree.h"
#include "multisort.h"

namespace KDebugger
{
TwoDSearch::TwoDSearch(DataGenerator* generator) :
    DataObserver(generator)
{
    updateData();
}

void TwoDSearch::updateData()
{
    data_generator_->getPtData(buf_);
    if (binsearch_1d_)
    {
        delete binsearch_1d_;
        binsearch_1d_ = nullptr;
    }

    if (range_2d_)
    {
        delete range_2d_;
        range_2d_ = nullptr;
    }
}

// For 1D
int TwoDSearch::getOneDRangeOri(double l, double r, bool _debug)
{
    size_t cnt = 0;
    std::vector<KPt> res;
    res_.swap(res);
    for (const auto& pt : buf_)
    {
        if (pt.y >= l && pt.y < r)
        {
            cnt++;
            res_.emplace_back(pt);
        }
    }
    if (_debug)
    {
        std::cout << "\n res of ori method:\n";
        reportRes();
        std::cout << std::endl;
    }
    return res_.size();
}
void TwoDSearch::reportRes()
{
    Sort_XS(res_.begin(), res_.end());
    printPntVec(res_);
}
// For 1D
int TwoDSearch::getOneDRange(double l, double r, bool _debug)
{
    // std::cout << "\n-----1D binnode search------" << std::endl;

    if (buf_.empty())
        return 0;
    KTimer timer;
    // BiSearch helper;
    if (!binsearch_1d_)
    {
        binsearch_1d_ = BiSearch::buildBinSearchTree(buf_);
        std::cout << "\nDataSize: " << buf_.size();
        std::cout << "\nBuild bintree in "; timer.timeFromBegin("");
    }
    std::vector<KPt> temp;
    res_.swap(temp);
    if (_debug)
    {
        std::cout << "\n ####tree start####\n";
        binsearch_1d_->printBinSearchTree(true);
        std::cout << "\n ####tree end####\n";
    }
    BiSearch::oneDRangeQuery(binsearch_1d_, l, r, res_);
    if (_debug)
    {
        reportRes();
    }
    return res_.size();
}

int TwoDSearch::getTwoDRangeOri(const KRegion& r, bool _debug)
{
    std::vector<KPt> temp;
    res_.swap(temp);
    KTimer timer;
    size_t cnt = 0;
    for (const auto& pt : buf_)
    {
        if (r.ptInRegion(pt))
        {
            res_.emplace_back(pt);
            cnt++;
        }
    }

    if (_debug)
    {
        std::cout << "\nOrigin: Get Quary in "; timer.timeFromLastSee("");
        std::cout << "res size:" << cnt << std::endl;
        std::cout << "\nres of origin method:\n";
        reportRes();
    }
    return res_.size();
}

int TwoDSearch::getTwoDRange(const std::vector<KPt>& corner_pnts,
    const KRegion& r, std::vector<KPt>& res)
{
    KTimer timer;
    size_t cnt = 0;
    for (const auto& pt : corner_pnts)
    {
        if (r.ptInRegion(pt))
        {
            res.emplace_back(pt);
            cnt++;
        }
    }
    return res.size();
}

int TwoDSearch::getTwoDExpendRange(const std::vector<KPt>& corner_pnts,
    const KRegion& r, std::vector<KPt>& res)
{
    KTimer timer;
    size_t cnt = 0;
    for (const auto& pt : corner_pnts)
    {
        if (r.ptInRegionExpend(pt))
        {
            res.emplace_back(pt);
            cnt++;
        }
    }
    return res.size();
}

int TwoDSearch::getTwoDRangeRangeTree(const KRegion& r, bool _debug)
{
    // std::cout << "\n-----2D Search Range2D------";

    KTimer timer;
    size_t cnt = 0;

    if (!range_2d_)
    {
        if (buf_.empty())
            return 0;
        range_2d_ = KRangeTree::buildRangeTree(buf_);

        std::cout << "\nSize: " << buf_.size() << " build range tree in "; timer.timeFromBegin("");
    }
    if (_debug)
    {
        std::cout << "######## all pnts ########\n";
        Sort_XS(buf_.begin(), buf_.end());
        printPntVec(buf_);
        std::cout << "######## all pnts ########\n";
        if (buf_.size() < 30)
        {
            std::cout << "\n ####tree start####" << std::endl;
            range_2d_->printBinSearchTree();
            std::cout << "\n ####tree end####" << std::endl;
        }
    }
    std::vector<KPt> temp;
    res_.swap(temp);
    KRangeTree::searchRangeTreeFromRoot(range_2d_, r, res_);

    if (_debug)
    {
        std::cout << "\nrange_2d: Get Quary in "
            ; timer.timeFromLastSee("");
        std::cout << "\nres size:" << res_.size() << std::endl;
        std::cout << "\nres of range tree method: ";
        reportRes();
    }
    return res_.size();
}
}