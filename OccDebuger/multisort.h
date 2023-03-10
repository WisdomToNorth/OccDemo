#pragma once

#include <thread>
#include <algorithm>
#include <vector>

#include "global.h"
#include "KPnt.h"

namespace KDebugger
{

static bool cmp_x_s(const KPt& a, const KPt& b)
{
    return (a.x < b.x) || (a.x == b.x && a.y > b.y);
}
static bool cmp_x_l(const KPt& a, const KPt& b)
{
    return (a.x > b.x) || (a.x == b.x && a.y < b.y);
}
static bool cmp_y_s(const KPt& a, const KPt& b)
{
    return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}
static bool cmp_y_l(const KPt& a, const KPt& b)
{
    return (a.y > b.y) || (a.y == b.y && a.x < b.x);
}

template<typename Iterator>
void F_XS(Iterator start, Iterator end)
{
    std::sort(start, end, cmp_x_s);
}
template<typename Iterator>
void F_XL(Iterator start, Iterator end)
{
    std::sort(start, end, cmp_x_l);
}
template<typename Iterator>
void F_YS(Iterator start, Iterator end)
{
    std::sort(start, end, cmp_y_s);
}
template<typename Iterator>
void F_YL(Iterator start, Iterator end)
{
    std::sort(start, end, cmp_y_l);
}

template <typename Iterator>
void merge_xs(Iterator start, int length, int size)
{
    for (int i = 1; i < size; i++)
        std::inplace_merge(start, start + length * i,
            start + length * (i + 1), cmp_x_s);
}
template <typename Iterator>
void merge_xl(Iterator start, int length, int size)
{
    for (int i = 1; i < size; i++)
        std::inplace_merge(start, start + length * i,
            start + length * (i + 1), cmp_x_l);
}
template <typename Iterator>
void merge_ys(Iterator start, int length, int size)
{
    for (int i = 1; i < size; i++)
        std::inplace_merge(start, start + length * i,
            start + length * (i + 1), cmp_y_s);
}
template <typename Iterator>
void merge_yl(Iterator start, int length, int size)
{
    for (int i = 1; i < size; i++)
        std::inplace_merge(start, start + length * i,
            start + length * (i + 1), cmp_y_l);
}

template<typename Iterator>
void Sort_XS(Iterator start, Iterator end)
{
    const size_t length = std::distance(start, end);

    auto max_thread = G_CoreCount(length);
    const size_t size = length / max_thread;
    std::vector<std::thread>threads;
    Iterator block_start = start;

    for (size_t i = 0; i < (max_thread - 1); i++)
    {
        Iterator block_end = block_start;
        std::advance(block_end, size);
        threads.emplace_back(F_XS<Iterator>, block_start, block_end);
        block_start = block_end;
    }
    F_XS(block_start, end);
    for (auto& i : threads)
        i.join();
    merge_xs(start, size, max_thread);
}

template<typename Iterator>
void Sort_XL(Iterator start, Iterator end)
{
    const size_t length = std::distance(start, end);

    auto max_thread = G_CoreCount(length);
    const size_t size = length / max_thread;
    std::vector<std::thread>threads;
    Iterator block_start = start;

    for (size_t i = 0; i < (max_thread - 1); i++)
    {
        Iterator block_end = block_start;
        std::advance(block_end, size);
        threads.emplace_back(F_XL<Iterator>, block_start, block_end);
        block_start = block_end;
    }
    F_XL(block_start, end);
    for (auto& i : threads)
        i.join();
    merge_xl(start, size, max_thread);
}
template<typename Iterator>
void Sort_YS(Iterator start, Iterator end)
{
    const size_t length = std::distance(start, end);

    auto max_thread = G_CoreCount(length);
    const size_t size = length / max_thread;
    std::vector<std::thread>threads;
    Iterator block_start = start;

    for (size_t i = 0; i < (max_thread - 1); i++)
    {
        Iterator block_end = block_start;
        std::advance(block_end, size);
        threads.emplace_back(F_YS<Iterator>, block_start, block_end);
        block_start = block_end;
    }
    F_YS(block_start, end);
    for (auto& i : threads)
        i.join();
    merge_ys(start, size, max_thread);
}

template<typename Iterator>
void Sort_YL(Iterator start, Iterator end)
{
    const size_t length = std::distance(start, end);

    auto max_thread = G_CoreCount(length);
    const size_t size = length / max_thread;
    std::vector<std::thread>threads;
    Iterator block_start = start;

    for (size_t i = 0; i < (max_thread - 1); i++)
    {
        Iterator block_end = block_start;
        std::advance(block_end, size);
        threads.emplace_back(F_YL<Iterator>, block_start, block_end);
        block_start = block_end;
    }
    F_YL(block_start, end);
    for (auto& i : threads)
        i.join();
    merge_yl(start, size, max_thread);
}
}