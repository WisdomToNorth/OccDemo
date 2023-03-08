#include "KBox.h"

#include <chrono>
#include <vector>
#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>

#include <QString>

#include <GCPnts_UniformAbscissa.hxx>

#include "stadfx.h"
#include "global.h"
#include "shapetools.h"
#include "BoundingBox.h"

namespace KDebugger
{
KBox::KBox(double x, double y, double sizex, double sizey, int type) :
    center_(KPt(x, y)), size_x_(sizex), size_y_(sizey)
{
    if (type / 2)
    {
        type_ = ObjType::Box;
    }
    else type_ = ObjType::Elips;

}

bool KBox::isOut(const KBox& rhs)const
{
    double cx = (size_x_ + rhs.size_x_) * 0.5;
    double cy = (size_y_ + rhs.size_y_) * 0.5;

    if (abs(rhs.center_.x - center_.x) > cx || abs(rhs.center_.y - center_.y) > cy)
        return true;
    else return false;
}

bool KBox::outBox(const KPt& pt)const
{
    double cx = (size_x_) * 0.5;
    double cy = (size_y_) * 0.5;

    if (abs(pt.x - center_.x) > cx || abs(pt.y - center_.y) > cy)
        return true;
    else return false;
}
bool KBox::outBoxWithSpacing(const KPt& pt)const
{
    double cx = (size_x_) * 0.5 + space_;
    double cy = (size_y_) * 0.5 + space_;

    if (abs(pt.x - center_.x) > cx || abs(pt.y - center_.y) > cy)
        return true;
    else return false;
}

KBoundingBox KBox::getBoundingbox()const
{
    KPt lb(center_.x - size_x_ * 0.5, center_.y - size_y_ * 0.5);
    KPt ru(center_.x + size_x_ * 0.5, center_.y + size_y_ * 0.5);
    return KBoundingBox(lb, ru);
}

KBoundingBox KBox::getSpaceBoundingbox()const
{
    KPt lb(center_.x - size_x_ * 0.5 - space_,
        center_.y - size_y_ * 0.5 - space_);

    KPt ru(center_.x + size_x_ * 0.5 + space_,
        center_.y + size_y_ * 0.5 + space_);
    return KBoundingBox(lb, ru);
}

bool KBox::isCrossWithKLine(const KLine& line)const
{
    return getBoundingbox().isCrossKLine(line);
}

bool KBox::isCrossWithKLineWithSpace(const KLine& line)const
{
    return getSpaceBoundingbox().isCrossKLine(line);
}
KBox::~KBox()
{

}
void KBox::show()
{
    switch (type_)
    {
    case KDebugger::KBox::ObjType::Elips:
        // drawElips();
        drawBox();
        drawSpacingBox();
        break;
    case KDebugger::KBox::ObjType::Box:
        drawBox();
        drawSpacingBox();
        break;
    default:
        break;
    }

}

Handle(AIS_TextLabel) KBox::getText()
{
    const KBox& box = *this;
    gp_Pnt cur(box.center_.x, box.center_.y, 0);
    Handle(AIS_TextLabel) text = new AIS_TextLabel();
    text->SetPosition(cur);
    std::string text_ = '{' + QString::number(box.val_).toStdString() + '}' +
        "\n(" + QString::number(box.center_.x).toStdString()
        + ',' + QString::number(box.center_.y).toStdString() + ')';
    text->SetText(text_.c_str());
    text->SetColor(Quantity_NOC_BLACK);
    text->SetFont("consolas");
    return text;

}

void KBox::drawElips()
{
    if (G_Context.IsNull()) return;
    gp_Dir dir = gp_Dir(0, 0, 1);
    gp_Dir dirx = gp_Dir(1, 0, 0);

    gp_Pnt loc(center_.x, center_.y, 0);
    try
    {
        gp_Elips ge;
        if (size_x_ < size_y_)//major must greater than minor
        {
            ge = gp_Elips(gp_Ax2(loc, dir, dirx), size_y_ * 0.5,
                size_x_ * 0.5);
        }
        else
        {
            ge = gp_Elips(gp_Ax2(loc, dir, dirx), size_x_ * 0.5,
                size_y_ * 0.5);
        }

        TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(ge);
        BRepBuilderAPI_MakeWire WW(e2);
        TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(WW);
        Handle(AIS_Shape) shp = new AIS_Shape(myFaceProfile);

        G_Context->Display(shp, false);

        Handle(AIS_TextLabel) text = getText();

        G_Context->Display(text, false);
    }
    catch (...)
    {

    }
}

void KBox::drawBox()
{
    if (G_Context.IsNull()) return;
    gp_Pnt lb(center_.x - size_x_ * 0.5, center_.y - size_y_ * 0.5, 0);
    gp_Pnt rb(center_.x + size_x_ * 0.5, center_.y - size_y_ * 0.5, 0);
    gp_Pnt lu(center_.x - size_x_ * 0.5, center_.y + size_y_ * 0.5, 0);
    gp_Pnt ur(center_.x + size_x_ * 0.5, center_.y + size_y_ * 0.5, 0);


    TopoDS_Wire aWire = OccTools::getWireFromFourPts(lb, rb, ur, lu);

    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(aWire);
    Handle(AIS_Shape) shp = new AIS_Shape(myFaceProfile);

    G_Context->Display(shp, false);

    Handle(AIS_TextLabel) text = getText();

    G_Context->Display(text, false);
}

void KBox::drawSpacingBox()
{
    if (G_Context.IsNull()) return;
    gp_Pnt lb(center_.x - size_x_ * 0.5 - space_,
        center_.y - size_y_ * 0.5 - space_, 0);
    gp_Pnt rb(center_.x + size_x_ * 0.5 + space_,
        center_.y - size_y_ * 0.5 - space_, 0);
    gp_Pnt lu(center_.x - size_x_ * 0.5 - space_,
        center_.y + size_y_ * 0.5 + space_, 0);
    gp_Pnt ur(center_.x + size_x_ * 0.5 + space_,
        center_.y + size_y_ * 0.5 + space_, 0);

    Handle(AIS_Shape) shp = new AIS_Shape(OccTools::getWireFromFourPts(lb, rb, ur, lu));
    shp->SetColor(Quantity_NOC_GRAY11);
    G_Context->Display(shp, false);

    Handle(AIS_TextLabel) text = getText();

    G_Context->Display(text, false);
}

}