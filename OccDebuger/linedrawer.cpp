#include "linedrawer.h"
#include "global.h"

#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

#include "KLogger.h"
#include "gptools.h"

namespace KDebugger
{

LineDrawer::LineDrawer(const gp_Pnt& pnt)
{
    pnt_list_.emplace_back(pnt);
    cur_mode_ = Mode::init;
}

bool LineDrawer::appendLine(const gp_Pnt& new_pnt)
{
    const gp_Pnt last_pnt = pnt_list_.back();
    if (last_pnt.IsEqual(new_pnt, 0.1))return false;
    pnt_list_.emplace_back(new_pnt);

    TopoDS_Edge anEdge1 = OccTools::drawLineByTwoPts(last_pnt, new_pnt);

    Handle(AIS_ColoredShape) line = new AIS_ColoredShape(anEdge1);
    line->SetWidth(3.0);
    G_Context->Display(line, true);
    viewmodel_vec_.emplace_back(line);
    return true;
}

void LineDrawer::drawTempLine(const gp_Pnt& new_pnt)
{
    const gp_Pnt last_pnt = pnt_list_.back();
    if (last_pnt.IsEqual(new_pnt, 0.1))return;

    TopoDS_Edge anEdge1 = OccTools::drawLineByTwoPts(last_pnt, new_pnt);

    G_Context->Remove(temp_line_, false);
    temp_line_ = new AIS_ColoredShape(anEdge1);
    G_Context->Display(temp_line_, 1, -1, true);
}

void LineDrawer::commitDraw(std::list<gp_Pnt>& pnts)
{
    pnts.swap(pnt_list_);
    removeTempViewModel();
}

void LineDrawer::cancelDraw()
{
    removeTempViewModel();

}

void LineDrawer::removeTempViewModel()
{
    for (auto& obj : viewmodel_vec_)
    {
        G_Context->Remove(obj, false);
    }
    G_Context->Remove(temp_line_, false);
    G_Context->UpdateCurrentViewer();
}
}