#include "PreLine.h"

#include "gptools.h"

#include <TopoDS_Edge.hxx>
#include "global.h"

namespace KDebugger
{
PrePline::PrePline(const std::list<gp_Pnt>& res) :
    raw_pnts_(res), aabbox_(KBoundingBox(res.front()))
{

}

void PrePline::draw()
{

}

void PrePline::drawRawPnts()
{

    auto cur = raw_pnts_.begin();
    auto cur_next = raw_pnts_.begin();
    std::advance(cur_next, 1);

    while (cur_next != raw_pnts_.end())
    {
        TopoDS_Edge anEdge1 = OccTools::drawLineByTwoPts(*cur, *cur_next);
        Handle(AIS_ColoredShape) line = new AIS_ColoredShape(anEdge1);
        line->SetWidth(3.0);
        line->SetColor(Quantity_NOC_BLACK);
        G_Context->Display(line, false);
        viewmodel_vec_.push_back(line);
        cur++;
        cur_next++;
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

void PrePline::updateAABB()
{
    for (const auto& pt : raw_pnts_)
    {
        aabbox_.update(KPt(pt.X(), pt.Y()));
    }
}

void PrePline::reGenerate(const std::vector<KBox>& context_info)
{
    updateAABB();
    std::vector<KBox> adjacent_objs_;
    for (const auto& obj : context_info)
    {
        if (!obj.getBoundingbox().isOut(aabbox_))
        {
            adjacent_objs_.emplace_back(obj);
        }
    }
    std::cout << "\nCount of adjacent obj: " << adjacent_objs_.size() << std::endl;
}
}