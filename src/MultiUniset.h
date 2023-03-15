﻿#pragma once

#include <vector>
#include "KBox.h"
#include "unionset.h"
#include "DataObserver.h"

namespace KDebugger
{

class MultiUniset :public DataObserver
{
public:
    MultiUniset(DataGenerator* view);

    void updateData()override;
    void badWay();
    void oneCoreUnionSet();
    void multiCoreUnionSet(int user_set_num);

    void caculateUnion(unsigned long long l_start, unsigned long long l_end, UnionFind& finder);
    static std::pair<int, int> getLoc(unsigned long long num);
    unsigned long long getThreadCount(unsigned long long datasize, int defnum);
    int handleUnionFinder(const UnionFind& finder, bool use_multi);


    template <typename Iterator>
    int handleUnionSetResult(Iterator first, Iterator last, const int index);

private:


    std::vector<KBox> buf_;
};

}