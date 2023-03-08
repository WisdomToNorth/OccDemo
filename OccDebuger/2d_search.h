#pragma once
#include <iostream>

#include <algorithm>

#include "BinSearchNode.h"
#include "DataObserver.h"
#include "KRegion.h"

namespace KDebugger
{

class TwoDSearch :public DataObserver
{

public:
    TwoDSearch(DataGenerator* generator);
    void updateData()override;
    void getOneDRange(double l, double r);
    void getOneDRangeOri(double l, double r);
    void getTwoDRangeTwoDSearch(const KRegion& r);
    void getTwoDRangeOri(const KRegion& r);
    void getTwoDRangeRangeTree(const KRegion& r);

private:
    std::vector<KPt> buf_;

    BinSearchNode* binsearch_1d_ = nullptr;
    BinSearchNode* kd_2d_ = nullptr;
    BinSearchNode* range_2d_ = nullptr;

};

}