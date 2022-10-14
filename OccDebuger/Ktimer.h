#pragma once
#include <chrono>
#include <iostream>

class K_Timer
{
public:
    K_Timer() :
        t1(std::chrono::steady_clock::now()),
        t2(std::chrono::steady_clock::now())
    {    }
    double timeFromBegin(bool cout_ = true)
    {
        t2 = std::chrono::steady_clock::now();
        double dr_ms = std::chrono::duration<double, std::micro>(t2 - t1).count() / 1000;
        if (cout_)
        {
            std::cout << "time from start: " << dr_ms << std::endl;
        }
        return dr_ms;
    }
    double timeFromLastSee(bool cout_ = true)
    {
        auto t3 = std::chrono::steady_clock::now();
        double dr_ms = std::chrono::duration<double, std::micro>(t3 - t2).count() / 1000;
        t2 = t3;
        if (cout_)
        {
            std::cout << "time from last: " << dr_ms << std::endl;
        }
        return dr_ms;
    }
private:
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::time_point t2;
};

