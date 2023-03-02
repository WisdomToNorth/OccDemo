#include "BoundingBox.h"

#include <gp_Pnt.hxx>

namespace KDebugger
{
KBoundingBox::KBoundingBox(const gp_Pnt& pt)
    :lb_(KPt(pt.X(), pt.Y())), ru_(KPt(pt.X(), pt.Y()))
{
}

KBoundingBox::KBoundingBox(const KBox& box)
{
    *this = box.getBoundingbox();
}

bool KBoundingBox::isOut(const KBoundingBox& rhs)const
{
    return this->lb_.x > rhs.ru_.x
        || this->lb_.y > rhs.ru_.y
        || this->ru_.x < rhs.lb_.x
        || this->ru_.y < rhs.lb_.y;
}

bool KBoundingBox::isCrossKLine(const KLine& line)const
{
    gp_Pnt lu(lb_.x, ru_.y, 0);
    gp_Pnt rb(ru_.x, lb_.y, 0);
    gp_Pnt lb(lb_.x, lb_.y, 0);
    gp_Pnt ru(ru_.x, ru_.y, 0);

    KLine l1(lb, rb), l2(lb, lu), l3(rb, ru), l4(lu, ru);
    return l1.isCross(line) || l2.isCross(line) ||
        l3.isCross(line) || l4.isCross(line);
}

void KBoundingBox::update(const KPt& pt)
{
    if (pt.x < this->lb_.x)this->lb_.x = pt.x;
    if (pt.x > this->ru_.x)this->ru_.x = pt.x;

    if (pt.y < this->lb_.y)this->lb_.y = pt.y;
    if (pt.y > this->ru_.y)this->ru_.y = pt.y;

}
}