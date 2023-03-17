#include "test_datastructure.h"
#include "KLine.h"

#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include <random>
#include "global.h"


#include "KPnt.h"
namespace KDebugger
{

bool KTest::testLineCross()//todo: use qt or google test
{
    std::vector<std::pair<KLine, KLine>> test_set;
    test_set.push_back(std::make_pair
    (KLine({ 0,0 }, { 1,1 }), KLine({ 1,0 }, { 0,1 })));//true
    test_set.push_back(std::make_pair
    (KLine({ 0,0 }, { 0,1 }), KLine({ 1,0 }, { 1,1 })));//false
    test_set.push_back(std::make_pair
    (KLine({ 0,0 }, { 1,0 }), KLine({ 0,0 }, { 1,1 })));//true:one point
    test_set.push_back(std::make_pair
    (KLine({ 0,0 }, { 1,1 }), KLine({ 0,2 }, { 1,0 })));//true
    test_set.push_back(std::make_pair
    (KLine({ 0,0 }, { 1,1 }), KLine({ 0,0 }, { 1,1 })));//same


    for (const auto& obj : test_set)
    {
        std::cout << "check res:"
            << obj.first.isCross(obj.second) << std::endl;
    }
    return false;
}

bool KTest::testKPnt2D(int seed)
{
    G_Random_Engine.seed(seed);
    std::vector<KPt> pnts;
    //std::uniform_real_distribution<double> size_rand_gen(0, 10);//尺寸随机范围
    std::uniform_int_distribution<int>size_rand_gen(0, 10);//尺寸随机范围


    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            pnts.push_back(KPt(size_rand_gen(G_Random_Engine),
                size_rand_gen(G_Random_Engine)));
        }
    }
    PntsSorted2D tes(pnts);
    tes.print();
    PntsSorted2D p1, p2;

    tes.getSubPntsByMidY(p1, p2);
    std::cout << "\np1:" << p1.confirmValid();
    p1.print();

    std::cout << "\np2:" << p2.confirmValid();
    p2.print();
    bool res = p1.confirmValid() && p2.confirmValid();
    std::cout << "res:" << res << std::endl;
    return res;
}
}