#include "KLine.h"

#include <algorithm>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

namespace KDebugger
{
KLine::KLine(const gp_Pnt& p1, const gp_Pnt& p2) :
    p1_(KPt(p1.X(), p1.Y())), p2_(KPt(p2.X(), p2.Y()))
{
}

bool KLine::isCross(const KLine& rhs)const
{
    const KLine& lhs = *this;
    //如果四条判断有一个为真，则代表两线段必不可交，否则应该进行第二步判断。

    if (std::max(lhs.p1_.x, lhs.p2_.x) < std::min(rhs.p1_.x, rhs.p2_.x) ||
        std::max(lhs.p1_.y, lhs.p2_.y) < std::min(rhs.p1_.y, rhs.p2_.y) ||
        std::max(rhs.p1_.x, rhs.p2_.x) < std::min(lhs.p1_.x, lhs.p2_.x) ||
        std::max(rhs.p1_.y, rhs.p2_.y) < std::min(lhs.p1_.y, lhs.p2_.y))
        return false;

    gp_Pnt A = gp_Pnt(lhs.p1_.x, lhs.p1_.y, 0);
    gp_Pnt B = gp_Pnt(lhs.p2_.x, lhs.p2_.y, 0);
    gp_Pnt C = gp_Pnt(rhs.p1_.x, rhs.p1_.y, 0);
    gp_Pnt D = gp_Pnt(rhs.p2_.x, rhs.p2_.y, 0);

    gp_Vec AD(A, D), CD(C, D), BD(B, D);
    gp_Vec CB(C, B), DB(D, B), AB(A, B);

    if (AD.Crossed(CD).Dot(BD.Crossed(CD)) > 0 ||
        CB.Crossed(AB).Dot(DB.Crossed(AB)) > 0)
        return false;
    return true;
}
}