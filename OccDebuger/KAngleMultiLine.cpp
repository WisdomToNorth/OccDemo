#include "KAngleMultiLine.h"

#include "gptools.h"
#include "KLine.h"

namespace KDebugger
{

KAngleMultiLine::KAngleMultiLine(const gp_Pnt& p1, const gp_Pnt& p2) :
    p1_(p1), p2_(p2)
{
}

KAngleMultiLine::KAngleMultiLine(const KPt& p1, const KPt& p2) :
    p1_(gp_Pnt(p1.x, p1.y, 0)), p2_(gp_Pnt(p2.x, p2.y, 0))
{
}

void KAngleMultiLine::cpuAvailable(double angle)
{
    res_ = OccTools::drawAngledLineByTwoPts(p1_, p2_, angle);

}

gp_Pnt KAngleMultiLine::getRes()
{
    //todo: it is believed to have res here
    if (coli_1_)return res_.back();
    else return res_.front();
}

bool KAngleMultiLine::checkColli(const KBox& box)
{
    KBox l_box = box;

    if (res_.empty())
    {
        KLine temp(p1_, p2_);
        coli_1_ = l_box.isCrossWithVal(temp);
        coli_2_ = coli_1_;
        return coli_1_ && coli_2_;
    }
    if (!coli_1_)
    {
        KLine l1_1(p1_, res_.front());
        KLine l1_2(p2_, res_.front());
        if (l_box.isCrossWithVal(l1_1) ||
            l_box.isCrossWithVal(l1_2))
            coli_1_ = true;
    }

    if (!coli_2_)
    {
        KLine l1_3(p1_, res_.back());
        KLine l1_4(p2_, res_.back());
        if (l_box.isCrossWithVal(l1_3) ||
            l_box.isCrossWithVal(l1_4))
            coli_2_ = true;
    }
    return coli_1_ && coli_2_;
}


}