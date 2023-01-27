
#include "public.h"
#include <chrono>
#include <vector>
#include <BRepAlgoAPI_Algo.hxx>
#include <iostream>
#include <ctime>
#include <random>


#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Ax2.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <TopoDS.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <Geom_Plane.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <BRepLib.hxx>
#include <TopoDS_Face.hxx>
#include <AIS_TextLabel.hxx>

#include <QString>


void drawData(const std::vector<KBox>& buff, std::vector<TopoDS_Face>& vecset,
    std::vector<Handle(AIS_TextLabel)>& labs)
{
    size_t testsize = buff.size();
    gp_Dir dir = gp_Dir(0, 0, 1);
    gp_Dir dirx = gp_Dir(1, 0, 0);

    for (auto i = 0; i < testsize; ++i)
    {
        const KBox& box = buff[i];
        gp_Pnt loc(box.X(), box.Y(), 0);
        gp_Elips ge(gp_Ax2(loc, dir, dirx), box.size_x * 0.5, box.size_y * 0.5);
        TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(ge);
        BRepBuilderAPI_MakeWire WW(e2);
        TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(WW);
        vecset.emplace_back(myFaceProfile);

        gp_Pnt cur(box.X(), box.Y(), 0);
        Handle(AIS_TextLabel) text = new AIS_TextLabel();
        text->SetPosition(cur);
        text->SetText(QString::number(i).toStdString().c_str());
        text->SetColor(Quantity_NOC_BLACK);
        text->SetFont("consolas");
        labs.emplace_back(text);
    }

}
