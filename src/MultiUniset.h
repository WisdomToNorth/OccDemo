#pragma once

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
    void oneCoreUnionSet(bool merge = true);
    void optUnionSet(int user_set_num);

    void multiCoreUnionSet(int user_set_num, bool merge = true);
    void caculateUnion(unsigned long long l_start, unsigned long long l_end, UnionFind& finder);
    unsigned long long getThreadCount(unsigned long long datasize, int defnum);
    int handleUnionFinder(const UnionFind& finder, bool use_multi);

    template <typename Iterator>
    int handleUnionSetResult(Iterator first, Iterator last, const int index);

    static std::pair<int, int> getLoc(unsigned long long num);

private:
    std::vector<KBox> buf_;


};
}