#include "KAngleMultiLine.h"

#include "gptools.h"
#include "KLine.h"
#include "KMathUtils.h"
#include <gp_Vec.hxx>

namespace KDebugger
{

KAngleMultiLine::KAngleMultiLine(const gp_Pnt &p1, const gp_Pnt &p2) :
    p1_(p1), p2_(p2)
{
    if (p1_.X() > p2_.X()) // confirm A is left to B
    {
        gp_Pnt temp = p1_;
        p1_ = p2_;
        p2_ = temp;
    }
}

KAngleMultiLine::KAngleMultiLine(const KPt &p1, const KPt &p2) :
    p1_(gp_Pnt(p1.x, p1.y, 0)), p2_(gp_Pnt(p2.x, p2.y, 0))
{
    if (p1_.X() > p2_.X()) // confirm A is left to B
    {
        gp_Pnt temp = p1_;
        p1_ = p2_;
        p2_ = temp;
    }
}

void KAngleMultiLine::cpuAvailable(double angle)
{
    res_ = drawAngledLineByTwoPts(p1_, p2_, angle);
}

gp_Pnt KAngleMultiLine::getRes() const
{
    return res_.front();
}

double cpuAngle(const gp_Pnt &p1, const gp_Pnt &p2, const gp_Pnt &pc)
{
    gp_Vec v1(pc, p1);
    gp_Vec v2(pc, p2);
    return v1.Angle(v2) * 180.0 / M_PI;
}

double KAngleMultiLine::getPreviousRes(const KAngleMultiLine &previous_line)
{
    return cpuAngle(res_.front(), previous_line.getRes(), p1_);
}

bool KAngleMultiLine::checkBetterRes(const KAngleMultiLine &previous_line)
{
    // this->p1 is equal to previous_line-> p2

    if (res_.size() == 2)
    {
        auto a1 = cpuAngle(res_.front(), previous_line.getRes(), p1_);
        auto a2 = cpuAngle(res_.back(), previous_line.getRes(), p1_);
        if (a1 < a2)
            res_.pop_front();
        else
            res_.pop_back();
    }
    return cpuAngle(res_.front(), previous_line.getRes(), p1_) > 90.0;
}

std::vector<TopoDS_Edge> KAngleMultiLine::getEdge()
{
    std::vector<TopoDS_Edge> res;
    if (res_.empty())
    {
        if (!p1_.IsEqual(p2_, 0.01))
        {
            res.emplace_back(getEdgeByTwoPts(p1_, p2_));
        }
    }
    else
    {
        if (!p1_.IsEqual(res_.front(), 0.01))
        {
            res.emplace_back(getEdgeByTwoPts(p1_, res_.front()));
        }
        if (!p2_.IsEqual(res_.front(), 0.01))
        {
            res.emplace_back(getEdgeByTwoPts(p2_, res_.front()));
        }
    }
    return res;
}

bool KAngleMultiLine::checkColli(const KBox &box)
{
    if (res_.empty())
    {
        return box.isCrossWithKLineWithSpace(KLine(p1_, p2_));
    }
    auto it = res_.begin();
    while (it != res_.end())
    {
        KLine l1_1(p1_, *it);
        KLine l1_2(p2_, *it);
        if (box.isCrossWithKLineWithSpace(l1_1) || box.isCrossWithKLineWithSpace(l1_2))
        {
            // res_.erase(it);
            return true; // 这里只找了一条路径。另一条注释掉了。程序崩溃，需要检查原因
        }

        it++;
    }

    return res_.empty();
}

// range of angle: (0~90)
// return: pnts: pA,pB,pC,pD

std::list<gp_Pnt> KAngleMultiLine::drawAngledLineByTwoPts(gp_Pnt pA,
                                                          gp_Pnt pB, const double &angle) // todo: handle divide 0
{
    double arc = angle * M_PI / 180.0;
    double tan_a = std::tan(arc);
    std::list<gp_Pnt> res; // { pA, pB };
    if (pA.X() > pB.X())   // confirm A is left to B
    {
        gp_Pnt temp = pA;
        pA = pB;
        pB = temp;
    }
    double dx = std::abs(pB.X() - pA.X());
    double dy = std::abs(pB.Y() - pA.Y());

    if (fEqual(dx, 0.0, 0.0001) || fEqual(dy, 0.0, 0.0001) || dy / dx == tan_a)
    {
        return res;
    }
    else if (dy / dx < tan_a)
    {
        res.push_back(gp_Pnt(pB.X() - dy / tan_a, pA.Y(), 0));
        res.push_back(gp_Pnt(pA.X() + dy / tan_a, pB.Y(), 0));
    }
    else // dy / dx > tan_a
    {
        if (pB.Y() > pA.Y())
        {
            res.push_back(gp_Pnt(pB.X(), pA.Y() + dx / tan_a, 0));
            res.push_back(gp_Pnt(pA.X(), pB.Y() - dx / tan_a, 0));
        }
        else
        {
            res.push_back(gp_Pnt(pB.X(), pA.Y() - dx / tan_a, 0));
            res.push_back(gp_Pnt(pA.X(), pB.Y() + dx / tan_a, 0));
        }
    }

    return res;
}

} // namespace KDebugger