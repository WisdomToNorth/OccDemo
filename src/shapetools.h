#pragma once

#include <vector>

#include <Quantity_Color.hxx>
#include <gp_Pnt.hxx>
#include <AIS_Trihedron.hxx>

class TopoDS_Wire;
class TopoDS_Edge;

namespace KDebugger
{
Handle(AIS_Trihedron) createOriginTrihedron();

Quantity_Color getRandomColor();

// pnts should be sorted
TopoDS_Wire getWireFromFourPts(const gp_Pnt &lb, const gp_Pnt &rb,
                               const gp_Pnt &ru, const gp_Pnt &lu);
std::vector<gp_Pnt> getEdgeEndPts(TopoDS_Edge shp);
TopoDS_Edge getCircleFromPt(const gp_Pnt &lb, double r = 0.5);
} // namespace KDebugger