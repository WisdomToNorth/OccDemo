#include "shapetools.h"

#include <AIS_Trihedron.hxx>
#include <Geom_Axis2Placement.hxx>
#include <TopoDS_Wire.hxx>
#include <GC_MakeSegment.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Tool.hxx>
#include <GC_MakeCircle.hxx>

#include "global.h"

namespace KDebugger
{
Handle(AIS_Trihedron) createOriginTrihedron()
{
    Handle(Geom_Axis2Placement) axis = new Geom_Axis2Placement(gp::XOY());
    Handle(AIS_Trihedron) aisTrihedron = new AIS_Trihedron(axis);
    aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
    aisTrihedron->SetDrawArrows(false);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
    aisTrihedron->SetLabel(Prs3d_DP_XAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_YAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "");
    aisTrihedron->SetSize(60);
    aisTrihedron->SetTransformPersistence(
        new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location()));
    aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
    aisTrihedron->SetInfiniteState(true);
    return aisTrihedron;
}

Quantity_Color getRandomColor()
{
    std::uniform_int_distribution<int> c(0, 5);
    int cur = c(KDebugger::G_Random_Engine);
    if (cur == 0)
        return Quantity_NOC_BLUE;
    else if (cur == 1)
        return Quantity_NOC_RED;
    else if (cur == 2)
        return Quantity_NOC_GREEN;
    else if (cur == 3)
        return Quantity_NOC_BROWN;
    else if (cur == 4)
        return Quantity_NOC_ORANGE;
    else
        return Quantity_NOC_BLUE3;
}

TopoDS_Wire getWireFromFourPts(const gp_Pnt &lb, const gp_Pnt &rb,
                               const gp_Pnt &ru, const gp_Pnt &lu)
{
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(lb, rb);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(rb, ru);
    Handle(Geom_TrimmedCurve) aSegment3 = GC_MakeSegment(ru, lu);
    Handle(Geom_TrimmedCurve) aSegment4 = GC_MakeSegment(lu, lb);
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment3);
    TopoDS_Edge anEdge4 = BRepBuilderAPI_MakeEdge(aSegment4);

    return BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3, anEdge4);
}

TopoDS_Edge getCircleFromPt(const gp_Pnt &lb, double r)
{
    Handle(Geom_Circle) se = GC_MakeCircle(gp_Ax1(lb, gp_Dir(0, 0, 1)), r);
    return BRepBuilderAPI_MakeEdge(se);
}

std::vector<gp_Pnt> getEdgeEndPts(TopoDS_Edge shp)
{
    std::vector<gp_Pnt> res;
    Standard_Real first, last;

    Handle(Geom_Curve) curve = BRep_Tool::Curve(shp, first, last);
    if (curve)
    {
        gp_Pnt pt1 = curve->Value(first);
        gp_Pnt pt2 = curve->Value(last);
        res.push_back(pt1);
        res.push_back(pt2);
    }
    return res;
}
} // namespace KDebugger