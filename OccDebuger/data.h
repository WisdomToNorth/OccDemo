﻿#pragma once
#include <iostream>
#include <Windows.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <thread>

struct KPt
{
    double x;
    double y;
    KPt(double _x, double _y) :x(_x), y(_y) {};
};

class KBox
{
public:
    //KBox() :center_(KPt(0, 0)) {}
    KBox(double x, double y, double sizex, double sizey) :
        center_(KPt(x, y)), size_x(sizex), size_y(sizey)
    {}
    bool isOut(const KBox& rhs)
    {
        double cx = (size_x + rhs.size_x) * 0.5;
        double cy = (size_y + rhs.size_y) * 0.5;
        //std::cout << "Cx:" << cx<<std::endl;
        //std::cout << "Cy:" << cy << std::endl<<std::endl;
        if (abs(rhs.center_.x - center_.x) > cx || abs(rhs.center_.y - center_.y) > cy)
            return true;
        else return false;
    }
    void mergeTest(const KBox&)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }
    void mergeTest(std::vector<KBox>&)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }

    double X()const { return center_.x; }
    double Y()const { return center_.y; }
    double size_x;
    double size_y;

private:
    KPt center_;
};
