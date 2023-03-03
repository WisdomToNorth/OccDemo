#pragma once

#include <vector>

#include <Quantity_Color.hxx>
#include <gp_Pnt.hxx>
//#include <TopoDS_Wire.hxx>

class Handle_AIS_Trihedron;
class TopoDS_Wire;
class TopoDS_Edge;

namespace OccTools
{
Handle_AIS_Trihedron createOriginTrihedron();

Quantity_Color getRandomColor();

//pnts should be sorted
TopoDS_Wire getWireFromFourPts(const gp_Pnt& lb, const gp_Pnt& rb,
    const gp_Pnt& ru, const gp_Pnt& lu);
std::vector<gp_Pnt> getEdgeEndPts(TopoDS_Edge shp);

}