#include "BoundingBox.h"

#include <gp_Pnt.hxx>

#include "KBox.h"
namespace KDebugger
{

KBoundingBox::KBoundingBox(const KPt &p1, const KPt &p2)
{
    lb_.x = std::min(p1.x, p2.x);
    lb_.y = std::min(p1.y, p2.y);
    ru_.x = std::max(p1.x, p2.x);
    ru_.y = std::max(p1.y, p2.y);

    rb_ = KPt(ru_.x, lb_.y);
    lu_ = KPt(lb_.x, ru_.y);
}

KBoundingBox::KBoundingBox(const gp_Pnt &pt) :
    lb_(KPt(pt.X(), pt.Y())), ru_(KPt(pt.X(), pt.Y()))
{
    rb_ = KPt(ru_.x, lb_.y);
    lu_ = KPt(lb_.x, ru_.y);
}

KBoundingBox::KBoundingBox(const KBox &box)
{
    *this = box.getBoundingbox();
}

bool KBoundingBox::isOut(const KBoundingBox &rhs) const
{
    return this->lb_.x > rhs.ru_.x
           || this->lb_.y > rhs.ru_.y
           || this->ru_.x < rhs.lb_.x
           || this->ru_.y < rhs.lb_.y;
}

bool KBoundingBox::isCrossKLine(const KLine &line) const
{
    gp_Pnt lu(lb_.x, ru_.y, 0);
    gp_Pnt rb(ru_.x, lb_.y, 0);
    gp_Pnt lb(lb_.x, lb_.y, 0);
    gp_Pnt ru(ru_.x, ru_.y, 0);

    KLine l1(lb, rb), l2(lb, lu), l3(rb, ru), l4(lu, ru);
    return l1.isCross(line) || l2.isCross(line) || l3.isCross(line) || l4.isCross(line);
}

bool KBoundingBox::ptInRegion(const KPt &pnt_) const //[ )
{
    return (pnt_.x >= lb_.x) && (pnt_.x < ru_.x) && (pnt_.y >= lb_.y) && (pnt_.y < ru_.y);
}
bool KBoundingBox::ptInRegionExpend(const KPt &pnt_) const //[ )
{
    return ((pnt_.x >= lb_.x) && (pnt_.x < ru_.x)) || ((pnt_.y >= lb_.y) && (pnt_.y < ru_.y));
}

void KBoundingBox::update(const KPt &pt)
{
    if (pt.x < this->lb_.x) this->lb_.x = pt.x;
    if (pt.x > this->ru_.x) this->ru_.x = pt.x;

    if (pt.y < this->lb_.y) this->lb_.y = pt.y;
    if (pt.y > this->ru_.y) this->ru_.y = pt.y;
}
} // namespace KDebugger