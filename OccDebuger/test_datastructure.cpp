#pragma once
#include "test_datastructure.h"
#include "KLine.h"

#include <vector>
#include <utility>
#include <map>
#include <iostream>

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

}