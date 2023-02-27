#include "PLine.h"

#include "gptools.h"

#include <TopoDS_Edge.hxx>
#include "global.h"

namespace KDebugger
{

void PrePline::draw()
{

}

void PrePline::drawRawPnts()
{
    size_t n = raw_pnts_.size();
    for (size_t i = 0; i < n - 1; ++i)
    {
        TopoDS_Edge anEdge1 = OccTools::drawLineByTwoPts(raw_pnts_[i], raw_pnts_[i + 1]);

        Handle(AIS_ColoredShape) line = new AIS_ColoredShape(anEdge1);
        line->SetWidth(3.0);
        line->SetColor(Quantity_NOC_BLACK);
        G_Context->Display(line, false);
        viewmodel_vec_.push_back(line);
    }
    G_Context->UpdateCurrentViewer();
}

void PrePline::hide()
{
    for (auto& obj : viewmodel_vec_)
    {
        G_Context->Remove(obj, false);
    }
    G_Context->UpdateCurrentViewer();
}
}