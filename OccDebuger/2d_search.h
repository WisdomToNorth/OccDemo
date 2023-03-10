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
    int getOneDRange(double l, double r, bool _debug = false);
    int getOneDRangeOri(double l, double r, bool _debug = false);
    int getTwoDRangeTwoDSearch(const KRegion& r);
    int getTwoDRangeOri(const KRegion& r);
    int getTwoDRangeRangeTree(const KRegion& r);
    void reportRes();
private:
    std::vector<KPt> buf_;
    std::vector<KPt> res_;
    BinSearchNode* binsearch_1d_ = nullptr;
    BinSearchNode* kd_2d_ = nullptr;
    BinSearchNode* range_2d_ = nullptr;

};

}