
#include "2d_search.h"
#include "DataGenerator.h"
#include "KTimer.h"
#include "BiSearch.h"
#include "KDSearch.h"
#include "RangeTree.h"

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

    KTimer timer;
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

    KTimer timer;
    BiSearch helper;
    if (!binsearch_1d_)
    {
        binsearch_1d_ = helper.buildBinSearchTree(buf_);
        //std::cout << "\n ####tree start####\n";
        //printBinSearchTree(binsearch_1d_, true);
        //std::cout << "\n ####tree end####\n";
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> res;
    helper.oneDRangeQuery(binsearch_1d_, l, r, res);

    std::cout << "\nGet Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << res.size() << std::endl;
    //printPntVec(res);
}

void KDTree::getTwoDRangeOri(const KRegion& r)
{
    std::cout << "\n\n-----------2D Search Origin------------" << std::endl;
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
    //for (auto& pt : buf)pt.print();
}

void KDTree::getTwoDRangeKDTree(const KRegion& r)
{
    std::cout << "\n\n-----------2D Search KD------------" << std::endl;

    KTimer timer;
    size_t cnt = 0;
    KDSearch helper;
    if (!kd_2d_)
    {
        kd_2d_ = helper.buildKDTree(buf_);
        //printBinSearchTree(kd_2d_);
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> buf = helper.searchKDTreeFromRoot(kd_2d_, r);
    std::cout << "\nkd_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << buf.size() << std::endl;
    //for (auto& pt : buf)pt.print();
}

void KDTree::getTwoDRangeRangeTree(const KRegion& r)
{
    std::cout << "\n\n-----------2D Search KD------------" << std::endl;

    KTimer timer;
    size_t cnt = 0;
    KRangeTree helper;
    if (!kd_range_)
    {
        kd_range_ = helper.buildRangeTree(buf_);
        //printBinSearchTree(kd_2d_);
        std::cout << "\nBuild tree in " << timer.timeFromBegin(false) << " ms\n";
    }
    std::vector<KPt> buf = helper.searchRangeTreeFromRoot(kd_range_, r);
    std::cout << "\nkd_2d: Get Quary in " << timer.timeFromLastSee(false) << " ms\n";
    std::cout << "res size:" << buf.size() << std::endl;
    //for (auto& pt : buf)pt.print();
}

}