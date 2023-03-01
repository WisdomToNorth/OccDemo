#include "mathUtils.h"
#include <math.h>

namespace OccTools
{

bool fequal(double avalue1, double avalue2, double atolerance)
{
    return fabs(avalue1 - avalue2) < atolerance;
}
}