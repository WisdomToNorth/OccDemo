#include "PreLine.h"

#include "gptools.h"

#include <TopoDS_Edge.hxx>
#include "global.h"
#include "KLine.h"

namespace KDebugger
{
PrePline::PrePline(const std::list<gp_Pnt>& res) :
    raw_pnts_(res), aabbox_(KBoundingBox(res.front()))
{

}

void PrePline::drawPnts(Quantity_Color color)
{

    auto cur = raw_pnts_.begin();
    auto cur_next = raw_pnts_.begin();
    std::advance(cur_next, 1);

    while (cur_next != raw_pnts_.end())
    {
        TopoDS_Edge anEdge1 = OccTools::drawLineByTwoPts(*cur, *cur_next);
        Handle(AIS_ColoredShape) line = new AIS_ColoredShape(anEdge1);
        line->SetWidth(3.0);
        line->SetColor(color);
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

void PrePline::updateColor(Quantity_Color color)
{
    for (auto& obj : viewmodel_vec_)
    {
        obj->SetColor(color);
    }
}

void PrePline::updateAABB()
{
    for (const auto& pt : raw_pnts_)
    {
        aabbox_.update(KPt(pt.X(), pt.Y()));
    }
}

void PrePline::normlizeSegment()
{
    updateColor();
    auto cur = raw_pnts_.begin();
    auto cur_next = raw_pnts_.begin();
    std::advance(cur_next, 1);

    while (cur_next != raw_pnts_.end())
    {
        std::vector<TopoDS_Edge> edges = OccTools::drawAngledLineByTwoPts(*cur, *cur_next);
        for (const auto& edge : edges)
        {
            Handle(AIS_ColoredShape) line = new AIS_ColoredShape(edge);
            line->SetWidth(3.0);
            G_Context->Display(line, false);
            viewmodel_vec_.push_back(line);
        }

        cur++;
        cur_next++;
    }

    G_Context->UpdateCurrentViewer();
}

void PrePline::reGenerate(const std::vector<KBox>& context_info)
{
    updateAABB();
    std::vector<KBox> adjacent_objs_;
    for (const auto& obj : context_info)
    {
        if (!obj.getBoundingbox().isOut(aabbox_))
        {
            adjacent_objs_.push_back(obj);
        }
    }
    std::cout << "\nCount of adjacent obj: " <<
        adjacent_objs_.size() << std::endl;

    std::list<gp_Pnt>pline_pnt;

    auto cur_vertax = raw_pnts_.begin();
    pline_pnt.emplace_back(*cur_vertax);

    auto cur_next = raw_pnts_.begin();
    cur_next++;

    while (cur_next != raw_pnts_.end())
    {

        if (GCheckCpuMode())return;//for debug

        bool colli = false;
        KLine to_check(*cur_vertax, *cur_next);
        for (const auto& obj : adjacent_objs_)
        {
            if (obj.isCross(to_check))
            {
                colli = true;
                std::cout << "\ncrash vertax: "
                    << OccTools::ptToStr(*cur_next)
                    << std::endl;
                break;
            }
        }
        if (!colli)//
        {
            cur_next++;
        }
        else
        {
            auto temp = cur_next;
            temp--;
            cur_vertax = temp;
            pline_pnt.emplace_back(*cur_vertax);
            std::cout << "\nvertax: " << OccTools::ptToStr(*cur_vertax)
                << std::endl;
        }
    }
    pline_pnt.emplace_back(raw_pnts_.back());

    raw_pnts_.swap(pline_pnt);

    this->updateColor();
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec;
    viewmodel_vec_.swap(viewmodel_vec_);
    this->drawPnts();
}
}