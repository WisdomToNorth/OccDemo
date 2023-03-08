#pragma once
#include <iostream>

#include <algorithm>

#include "BinSearchNode.h"
#include "DataObserver.h"
#include "KRegion.h"

namespace KDebugger
{



class KDTree :public DataObserver
{

public:
    KDTree(DataGenerator* generator);
    void updateData()override;
    void getOneDRange(double l, double r);
    void getOneDRangeOri(double l, double r);
    void getTwoDRangeKDTree(const KRegion& r);
    void getTwoDRangeOri(const KRegion& r);
    void getTwoDRangeRangeTree(const KRegion& r);

private:
    std::vector<KPt> buf_;
    BinSearchNode* binsearch_1d_ = nullptr;
    BinSearchNode* kd_2d_ = nullptr;
    BinSearchNode* kd_range_ = nullptr;

};

}