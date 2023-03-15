
#include "2d_search.h"
#include "DataGenerator.h"
#include "KTimer.h"
#include "BiSearch.h"
#include "KDSearch.h"
#include "RangeTree.h"
#include "multisort.h"

namespace KDebugger
{
TwoDSearch::TwoDSearch(DataGenerator* generator) :DataObserver(generator)
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
    if (kd_2d_)
    {
        delete kd_2d_;
        kd_2d_ = nullptr;
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
    //std::cout << "\n-----1D binnode search------" << std::endl;

    KTimer timer;
    BiSearch helper;
    if (!binsearch_1d_)
    {
        binsearch_1d_ = helper.buildBinSearchTree(buf_);
        std::cout << "\nDataSize: " << buf_.size();
        std::cout << "\nBuild bintree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt>temp;
    res_.swap(temp);
    if (_debug)
    {
        std::cout << "\n ####tree start####\n";
        binsearch_1d_->printBinSearchTree();
        std::cout << "\n ####tree end####\n";

    }
    helper.oneDRangeQuery(binsearch_1d_, l, r, res_);
    if (_debug)
    {
        reportRes();
    }
    return res_.size();
}

int TwoDSearch::getTwoDRangeOri(const KRegion& r, bool _debug)
{
    // std::cout << "\n-----2D Search Origin-----";

    std::vector<KPt>temp;
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
    std::cout << "\nOrigin: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << cnt << std::endl;
    if (_debug)
    {
        std::cout << "\nres of origin method:\n";
        reportRes();
    }
    return res_.size();
}

int TwoDSearch::getTwoDRangeKDSearch(const KRegion& r, bool _debug)
{
    // std::cout << "\n-----2D Search KD------";

    KTimer timer;
    size_t cnt = 0;
    KDSearch helper;
    if (!kd_2d_)
    {
        kd_2d_ = helper.buildTwoDSearch(buf_);
        std::cout << "\nDataSize: " << buf_.size();
        std::cout << "\nBuild kd tree in " << timer.timeFromBegin(false) << " ms" << std::endl;
    }

    if (_debug)
    {
        std::cout << "\n ####tree start####" << std::endl;
        kd_2d_->printBinSearchTree();
        std::cout << "\n ####tree end####" << std::endl;
    }
    std::vector<KPt> buf = helper.searchTwoDSearchFromRoot(kd_2d_, r);
    std::cout << "\nkd_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << buf.size() << std::endl;
    res_.swap(buf);

    if (_debug)
    {
        std::cout << "\nres of kd tree method:";
        reportRes();
    }

    return res_.size();
}

int TwoDSearch::getTwoDRangeRangeTree(const KRegion& r, bool _debug)
{
    //std::cout << "\n-----2D Search Range2D------";

    KTimer timer;
    size_t cnt = 0;
    KRangeTree helper;
    if (!range_2d_)
    {
        range_2d_ = helper.buildRangeTree(buf_);
        std::cout << "\nDataSize: " << buf_.size();
        std::cout << "\nBuild range tree in " << timer.timeFromBegin(false)
            << " ms" << std::endl;
    }
    if (_debug)
    {
        std::cout << "\n ####tree start####" << std::endl;
        range_2d_->printBinSearchTree();
        std::cout << "\n ####tree end####" << std::endl;
    }
    std::vector<KPt> temp;
    res_.swap(temp);
    helper.searchRangeTreeFromRoot(range_2d_, r, res_);
    std::cout << "\nrange_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << res_.size() << std::endl;

    if (_debug)
    {
        std::cout << "\nres of range tree method: ";
        reportRes();
    }
    return res_.size();
}
}