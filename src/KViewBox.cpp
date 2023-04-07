#include "KViewBox.h"

#include "stadfx.h"
#include "shapetools.h"
#include "global.h"

namespace KDebugger
{

KViewBox::KViewBox(double x, double y, double size) :
    KBox(x, y, size, size, 1)
{
}
KViewBox::~KViewBox()
{
    this->temphide();
}

void KViewBox::tempshow()
{
    if (G_Context.IsNull()) return;
    gp_Pnt lb(center_.x - size_x_ * 0.5, center_.y - size_y_ * 0.5, 0);
    gp_Pnt rb(center_.x + size_x_ * 0.5, center_.y - size_y_ * 0.5, 0);
    gp_Pnt lu(center_.x - size_x_ * 0.5, center_.y + size_y_ * 0.5, 0);
    gp_Pnt ur(center_.x + size_x_ * 0.5, center_.y + size_y_ * 0.5, 0);

    TopoDS_Wire aWire = OccTools::getWireFromFourPts(lb, rb, ur, lu);

    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(aWire);
    temp_obj_ = new AIS_Shape(myFaceProfile);

    G_Context->Display(temp_obj_, true);
}

void KViewBox::temphide()
{
    if (temp_obj_.IsNull()) return;
    G_Context->Remove(temp_obj_, true);
}
} // namespace KDebugger