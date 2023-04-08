#include "kMathUtils.h"

#include <cmath>

namespace KDebugger
{
bool fEqual(double avalue1, double avalue2, double atolerance)
{
    return std::fabs(avalue1 - avalue2) < atolerance;
}
} // namespace KDebugger