#include "K_box.h"

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
#include "KLine.h"
#include "BoundingBox.h"

namespace KDebugger
{
KBox::KBox(double x, double y, double sizex, double sizey, int type) :
    center_(KPt(x, y)), size_x(sizex), size_y(sizey)
{
    if (type / 2)
    {
        type_ = ObjType::Box;
    }
    else type_ = ObjType::Elips;

}

bool KBox::isOut(const KBox& rhs)const
{
    double cx = (size_x + rhs.size_x) * 0.5;
    double cy = (size_y + rhs.size_y) * 0.5;

    if (abs(rhs.center_.x - center_.x) > cx || abs(rhs.center_.y - center_.y) > cy)
        return true;
    else return false;
}

KBoundingBox KBox::getBoundingbox()const
{
    KPt lb(center_.x - size_x * 0.5, center_.y - size_y * 0.5);
    KPt ur(center_.x + size_x * 0.5, center_.y + size_y * 0.5);
    return KBoundingBox(lb, ur);
}

bool KBox::isCross(const KLine& line)const
{
    gp_Pnt lb(center_.x - size_x * 0.5, center_.y - size_y * 0.5, 0);
    gp_Pnt rb(center_.x + size_x * 0.5, center_.y - size_y * 0.5, 0);
    gp_Pnt lu(center_.x - size_x * 0.5, center_.y + size_y * 0.5, 0);
    gp_Pnt ur(center_.x + size_x * 0.5, center_.y + size_y * 0.5, 0);
    KLine l1(lb, rb), l2(lb, lu), l3(rb, ur), l4(lu, ur);
    return l1.isCross(line) || l2.isCross(line) ||
        l3.isCross(line) || l4.isCross(line);
}

void KBox::show()
{
    switch (type_)
    {
    case KDebugger::KBox::ObjType::Elips:
        drawElips();
        break;
    case KDebugger::KBox::ObjType::Box:
        drawBox();
        break;
    default:
        break;
    }

}
void KBox::drawElips()
{
    if (G_Context.IsNull()) return;
    gp_Dir dir = gp_Dir(0, 0, 1);
    gp_Dir dirx = gp_Dir(1, 0, 0);

    const KBox& box = *this;
    gp_Pnt loc(box.X(), box.Y(), 0);
    try
    {
        gp_Elips ge;
        if (box.size_x < box.size_y)//major must greater than minor
        {
            ge = gp_Elips(gp_Ax2(loc, dir, dirx), box.size_y * 0.5,
                box.size_x * 0.5);
        }
        else
        {
            ge = gp_Elips(gp_Ax2(loc, dir, dirx), box.size_x * 0.5,
                box.size_y * 0.5);
        }

        TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(ge);
        BRepBuilderAPI_MakeWire WW(e2);
        TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(WW);
        Handle(AIS_Shape) shp = new AIS_Shape(myFaceProfile);

        G_Context->Display(shp, false);
        gp_Pnt cur(box.X(), box.Y(), 0);
        Handle(AIS_TextLabel) text = new AIS_TextLabel();
        text->SetPosition(cur);
        std::string text_ = '{' + QString::number(this->val_).toStdString() + '}' +
            '\n' + QString::number(box.X()).toStdString()
            + ',' + QString::number(box.Y()).toStdString();
        text->SetText(text_.c_str());
        text->SetColor(Quantity_NOC_BLACK);
        text->SetFont("consolas");
        G_Context->Display(text, false);
    }
    catch (...)
    {

    }
}

void KBox::drawBox()
{
    if (G_Context.IsNull()) return;
    gp_Pnt lb = gp_Pnt(center_.x - size_x * 0.5, center_.y - size_y * 0.5, 0);
    gp_Pnt rb = gp_Pnt(center_.x + size_x * 0.5, center_.y - size_y * 0.5, 0);
    gp_Pnt lu = gp_Pnt(center_.x - size_x * 0.5, center_.y + size_y * 0.5, 0);
    gp_Pnt ur = gp_Pnt(center_.x + size_x * 0.5, center_.y + size_y * 0.5, 0);

    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(lb, rb);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(rb, ur);
    Handle(Geom_TrimmedCurve) aSegment3 = GC_MakeSegment(ur, lu);
    Handle(Geom_TrimmedCurve) aSegment4 = GC_MakeSegment(lu, lb);
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment3);
    TopoDS_Edge anEdge4 = BRepBuilderAPI_MakeEdge(aSegment4);

    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3, anEdge4);

    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(aWire);
    Handle(AIS_Shape) shp = new AIS_Shape(myFaceProfile);

    G_Context->Display(shp, false);

    const KBox& box = *this;
    gp_Pnt cur(box.X(), box.Y(), 0);
    Handle(AIS_TextLabel) text = new AIS_TextLabel();
    text->SetPosition(cur);
    std::string text_ = '{' + QString::number(this->val_).toStdString() + '}' +
        '\n' + QString::number(box.X()).toStdString()
        + ',' + QString::number(box.Y()).toStdString();
    text->SetText(text_.c_str());
    text->SetColor(Quantity_NOC_BLACK);
    text->SetFont("consolas");
    G_Context->Display(text, false);
}

}