#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace KDebugger
{
class KTimer
{
public:
    KTimer() : t1_(std::chrono::steady_clock::now()), t2_(t1_)
    {
    }

    void resetTimer()
    {
        t1_ = std::chrono::steady_clock::now();
        t2_ = t1_;
    }

    template <typename T = std::milli>
    double timeFromBegin(const std::string &str = "\nTime from start: ")
    {
        double res = timeFromBeginP<T>();
        if (!str.empty())
        {
            std::cout << str << res << " ms" << std::endl;
        }
        return res;
    }
    template <typename T = std::milli>
    double timeFromLastSee(const std::string &str = "\nTime from last see: ")
    {
        double dr_ms = timeFromLastSeeP<T>();
        if (!str.empty())
        {
            std::cout << str << dr_ms << " ms" << std::endl;
        }
        return dr_ms;
    }

private:
    template <typename T>
    double timeFromBeginP()
    {
        t2_ = std::chrono::steady_clock::now();
        double dr_ms = std::chrono::duration<double, T>(t2_ - t1_).count();

        return dr_ms;
    }

    template <typename T>
    double timeFromLastSeeP()
    {
        auto t3 = std::chrono::steady_clock::now();
        double dr_ms = std::chrono::duration<double, T>(t3 - t2_).count();
        t2_ = t3;

        return dr_ms;
    }

private:
    std::chrono::steady_clock::time_point t1_;
    std::chrono::steady_clock::time_point t2_;
};
} // namespace KDebugger