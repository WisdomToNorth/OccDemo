#include "test_datastructure.h"
#include "KLine.h"

#include <vector>
#include <utility>
#include <map>
#include <iostream>

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

bool KTest::testKPnt2D()
{
    std::vector<KPt> pnts{ KPt{ 0, 2 }, KPt{ 1, 3 }, KPt{ 2, 0 }, KPt{ 5, 0 },
        KPt{ 2, 2 }, KPt{ 3, 1 }, KPt{ 0, 4 }, KPt{ 0, 6 }, KPt{ 0, 8 },
        KPt{ 2, 7 }, KPt{ 3, 7 }, KPt{ 4, 4 }, KPt{ 2, 10 }, KPt{ 4, 7 } };
    PntsSorted2D tes(pnts);


    return true;
}
}