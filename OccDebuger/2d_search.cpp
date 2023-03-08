
#include "2d_search.h"
#include "DataGenerator.h"
#include "KTimer.h"
#include "BiSearch.h"
#include "KDSearch.h"
#include "RangeTree.h"

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
void TwoDSearch::getOneDRangeOri(double l, double r)
{
    std::cout << "\n-----------1D Search------------" << std::endl;

    KTimer timer;
    size_t cnt = 0;
    std::vector<KPt> res;
    for (const auto& pt : buf_)
    {
        if (pt.y >= l && pt.y < r)
        {
            cnt++;
            res.emplace_back(pt);
        }

    }

    std::cout << "\nOrigin: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << cnt << std::endl;
    //printPntVec(res);

}

// For 1D
void TwoDSearch::getOneDRange(double l, double r)
{
    std::cout << "\n-----------1D binnode search------------" << std::endl;

    KTimer timer;
    BiSearch helper;
    if (!binsearch_1d_)
    {
        binsearch_1d_ = helper.buildBinSearchTree(buf_);
        //std::cout << "\n ####tree start####\n";
        //binsearch_1d_->printBinSearchTree();
        //std::cout << "\n ####tree end####\n";
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> res;
    helper.oneDRangeQuery(binsearch_1d_, l, r, res);

    std::cout << "\nGet Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << res.size() << std::endl;
    //printPntVec(res);
}

void TwoDSearch::getTwoDRangeOri(const KRegion& r)
{
    std::cout << "\n-----------2D Search Origin------------";
    std::vector<KPt> buf;
    KTimer timer;
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
}

void TwoDSearch::getTwoDRangeTwoDSearch(const KRegion& r)
{
    std::cout << "\n-----------2D Search KD------------";

    KTimer timer;
    size_t cnt = 0;
    KDSearch helper;
    if (!kd_2d_)
    {
        kd_2d_ = helper.buildTwoDSearch(buf_);
        //printBinSearchTree(kd_2d_);
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> buf = helper.searchTwoDSearchFromRoot(kd_2d_, r);
    std::cout << "\nkd_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << buf.size() << std::endl;
    //for (auto& pt : buf)pt.print();
}

void TwoDSearch::getTwoDRangeRangeTree(const KRegion& r)
{
    std::cout << "\n-----------2D Search Range2D------------";

    KTimer timer;
    size_t cnt = 0;
    KRangeTree helper;
    if (!range_2d_)
    {
        range_2d_ = helper.buildRangeTree(buf_);
        //range_2d_->printBinSearchTree();
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> buf;
    helper.searchRangeTreeFromRoot(range_2d_, r, buf);
    std::cout << "\nrange_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << buf.size() << std::endl;
    //printPntVec(buf);
}
}