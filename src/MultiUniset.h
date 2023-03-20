﻿#pragma once

#include <vector>
#include <set>

#include "KBox.h"
#include "KPnt.h"
#include "unionset.h"
#include "DataObserver.h"

namespace KDebugger
{
struct BinSearchNode;
class MultiUniset :public DataObserver
{

public:
    MultiUniset(DataGenerator* view);

    void updateData()override;
    void badWay();

    int oneCoreUnionSet();
    int optUnionSet(bool _multi, bool _debug = false);
    int multiCoreUnionSet(int user_set_num, bool _debug = false);

private:
    void caculateUnion(unsigned long long l_start, unsigned long long l_end, UnionFind& finder);
    unsigned long long getThreadCount(unsigned long long datasize, int defnum);
    int handleUnionFinder(const UnionFind& finder, bool use_multi, bool _debug = false);

    template <typename Iterator>
    int handleUnionSetResult(Iterator first, Iterator last, const int index);

    static std::pair<int, int> getLoc(unsigned long long num);

private:
    std::vector<KBox> buf_;


};
}