#pragma once

#include <vector>
#include "K_box.h"
#include "unionset.h"

class CadView;
class MultiUniset
{
public:
    MultiUniset(CadView* view) :viewer_(view) { }

    void reGenerateData(int rowcnt, int colcnt, double dis);
    void badWay();
    void oneCoreUnionSet();
    void multiCoreUnionSet(int user_set_num);
    void viewData();

    void generateTestData(std::vector<KBox>& buffer,
        int testrow, int testcol, double distance);

    void caculateUnion(unsigned long long l_start, unsigned long long l_end, UnionFind& finder);
    static std::pair<int, int> getLoc(unsigned long long num);
    unsigned long long getThreadCount(unsigned long long datasize, int defnum);
    int handleUnionFinder(const UnionFind& finder, bool use_multi);


    template <typename Iterator>
    int handleUnionSetResult(Iterator first, Iterator last, const int index);

private:

    CadView* viewer_;

    std::vector<KBox> buf_;
};

