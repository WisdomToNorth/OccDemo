#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace KDebugger
{
class KTimer
{
public:
    KTimer() :t1_(std::chrono::steady_clock::now()), t2_(t1_) {}

    double timeFromBegin(const std::string& str = "\nTime from start: ")
    {
        double res = timeFromBeginP();
        std::cout << str << res << " ms" << std::endl;
        return res;
    }

    double timeFromLastSee(const std::string& str = "\nTime from last see: ")
    {
        double dr_ms = timeFromLastSeeP();
        std::cout << str << dr_ms << " ms" << std::endl;
        return dr_ms;
    }

private:
    double timeFromBeginP()
    {
        t2_ = std::chrono::steady_clock::now();
        double dr_ms = std::chrono::duration<double, std::milli>(t2_ - t1_).count();

        return dr_ms;
    }
    double timeFromLastSeeP()
    {
        auto t3 = std::chrono::steady_clock::now();
        double dr_ms = std::chrono::duration<double, std::milli>(t3 - t2_).count();
        t2_ = t3;

        return dr_ms;
    }
private:
    std::chrono::steady_clock::time_point t1_;
    std::chrono::steady_clock::time_point t2_;

};
}