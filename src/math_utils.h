
#pragma once
#ifndef _USER_MATH_H_
#define _USER_MATH_H_

#ifdef _WIN32
#include <cmath>
#endif

#include <algorithm>
#include <math.h>
#include <utility>

namespace RFBase
{
inline bool fuzzyEqual(float f1, float f2)
{
    return (std::abs(f1 - f2) * 100000.f <= (std::min)(std::abs(f1), std::abs(f2)));
}

// Same as fuzzyEqual(float, float) but for double-precision values
inline bool fuzzyEqual(double d1, double d2)
{
    return (std::abs(d1 - d2) * 1000000000000. <= (std::min)(std::abs(d1), std::abs(d2)));
}

/**
 * Maps a value from one range to another.
 *
 * Given a value `val`, this function maps it from the original range `[omin, omax]`
 * to the new range `[nmin, nmax]`. The function returns the mapped value.
 *
 * @param val The value to map.
 * @param omin The minimum value of the original range.
 * @param omax The maximum value of the original range.
 * @param nmin The minimum value of the new range.
 * @param nmax The maximum value of the new range.
 * @return The mapped value.
 */
template <typename T, typename T1, typename T2, typename T3, typename T4>
double mappedValue(T val, T1 omin, T2 omax, T3 nmin, T4 nmax)
{
    const auto dist1 = static_cast<double>(omax - omin);
    const auto dist2 = nmax - nmin;
    const auto distVal = val - omin;
    return ((distVal * dist2) / dist1) + nmin;
}

template <typename T, typename T1, typename T2>
double toPercent(T val, T1 omin, T2 omax)
{
    return mappedValue(val, omin, omax, 0, 100);
}

// result = a + t * (b - a)
template <typename T, typename U>
T lerp(T a, T b, U t)
{
    return std::lerp(a, b, t);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/intcmp
template <typename T, typename U>
constexpr bool cmpEqual(T t, U u) noexcept
{
    return std::cmp_equal(t, u);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/intcmp
template <typename T, typename U>
constexpr bool cmpNotEqual(T t, U u) noexcept
{
    return std::cmp_not_equal(t, u);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/intcmp
template <typename T, typename U>
constexpr bool cmpLess(T t, U u) noexcept
{
    return std::cmp_less(t, u);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/intcmp
template <typename T, typename U>
constexpr bool cmpGreater(T t, U u) noexcept
{
    return std::cmp_greater(t, u);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/intcmp
template <typename T, typename U>
constexpr bool cmpLessEqual(T t, U u) noexcept
{
    return std::cmp_less_equal(t, u);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/intcmp
template <typename T, typename U>
constexpr bool cmpGreaterEqual(T t, U u) noexcept
{
    return std::cmp_greater_equal(t, u);
}

// Implementation from https://en.cppreference.com/w/cpp/utility/in_range
template <typename R, typename T>
constexpr bool inRange(T t) noexcept
{
    return std::in_range<T>(t);
}

} // namespace RFBase
#endif
