#include "PreLine.h"

#include "gptools.h"

#include <TopoDS_Edge.hxx>
#include "global.h"
#include "KLine.h"
#include "KAngleMultiLine.h"

namespace KDebugger
{
PrePline::PrePline(const std::list<gp_Pnt>& res) :
    raw_pnts_(res), aabbox_(KBoundingBox(res.front()))
{

}

void PrePline::drawRawPnts(Quantity_Color color)
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

void PrePline::drawPnts(Quantity_Color color)
{

    auto cur = raw_pnts_.begin();
    auto cur_next = raw_pnts_.begin();
    std::advance(cur_next, 1);

    while (cur_next != raw_pnts_.end())
    {
        if (cur->IsEqual(*cur_next, 0.01))
        {
            cur++;
            cur_next++;
            continue;

        }

        KAngleMultiLine to_check(*cur, *cur_next);
        to_check.cpuAvailable(45);

        for (const KBox& obj : adjacent_objs_)
        {
            to_check.checkColli(obj);
        }
        if (to_check.resSize())
        {
            gp_Pnt mid = to_check.getRes();
            if (!cur->IsEqual(mid, 0.01))
            {
                TopoDS_Edge e1 = OccTools::drawLineByTwoPts(*cur, mid);
                Handle(AIS_ColoredShape) line = new AIS_ColoredShape(e1);
                line->SetWidth(3.0);
                line->SetColor(color);
                G_Context->Display(line, false);
                viewmodel_vec_.push_back(line);
            }
            if (!cur_next->IsEqual(mid, 0.01))
            {
                TopoDS_Edge e2 = OccTools::drawLineByTwoPts(mid, *cur_next);

                Handle(AIS_ColoredShape) line2 = new AIS_ColoredShape(e2);
                line2->SetWidth(3.0);
                line2->SetColor(color);
                G_Context->Display(line2, false);
                viewmodel_vec_.push_back(line2);
            }
        }
        else
        {

            TopoDS_Edge e1 = OccTools::drawLineByTwoPts(*cur, *cur_next);
            Handle(AIS_ColoredShape) line = new AIS_ColoredShape(e1);
            line->SetWidth(3.0);
            line->SetColor(color);
            G_Context->Display(line, false);
            viewmodel_vec_.push_back(line);
        }
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

void PrePline::printAllPts()
{
    std::cout << "\n######";
    auto cur = raw_pnts_.begin();
    while (cur != raw_pnts_.end())
    {
        std::cout << "\n " << OccTools::ptToStr(*cur);
        ++cur;
    }
    std::cout << "\n######";
}

void PrePline::normlizeSegment()
{
    printAllPts();
    auto cur = raw_pnts_.begin();
    auto cur_next = raw_pnts_.begin();
    std::advance(cur_next, 1);

    while (cur_next != raw_pnts_.end())
    {
        const std::vector<gp_Pnt>& pnts = OccTools::drawAngledLineByTwoPts(*cur, *cur_next);
        if (pnts.empty())//vertical or horizon
        {
            /*TopoDS_Edge edge = OccTools::drawLineByTwoPts(pnts[0], pnts[1]);
            Handle(AIS_ColoredShape) line = new AIS_ColoredShape(edge);
            line->SetWidth(3.0);
            G_Context->Display(line, false);
            viewmodel_vec_.push_back(line);*/
            cur++;
            cur_next++;
        }
        else//size == 4
        {

            std::cout << "\nbefore insert: " << OccTools::ptToStr(*cur) << std::endl;
            std::cout << "next before insert: " << OccTools::ptToStr(*cur_next) << std::endl;
            std::cout << "insert: " << OccTools::ptToStr(pnts[0]) << std::endl;
            cur++;
            raw_pnts_.insert(cur, pnts[0]);
            std::cout << "after insert: " << OccTools::ptToStr(*cur) << std::endl;
            std::cout << "next after insert: " << OccTools::ptToStr(*cur_next) << std::endl;
            //cur++;
            cur_next++;
            printAllPts();
        }
    }
    std::cout << "\nAfter insert: ";

    this->updateColor();
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec;
    viewmodel_vec_.swap(viewmodel_vec_);
    this->drawPnts();
    G_Context->UpdateCurrentViewer();
}

void PrePline::caculateEnviroment(const std::vector<KBox>& context_info)
{
    updateAABB();
    for (const auto& obj : context_info)
    {
        if (!obj.getBoundingbox().isOut(aabbox_))
        {
            adjacent_objs_.push_back(obj);
        }
    }
    std::cout << "\nCount of adjacent obj: " <<
        adjacent_objs_.size() << std::endl;
}

void PrePline::reGenerate(const std::vector<KBox>& context_info)
{
    caculateEnviroment(context_info);

    std::list<gp_Pnt>pline_pnt;

    auto cur_vertax = raw_pnts_.begin();
    pline_pnt.emplace_back(*cur_vertax);

    auto cur_next = raw_pnts_.begin();
    cur_next++;

    while (cur_next != raw_pnts_.end())
    {

        if (GCheckCpuMode())return;//for debug

        bool colli = false;
        KAngleMultiLine to_check(*cur_vertax, *cur_next);
        to_check.cpuAvailable(45);

        for (const KBox& obj : adjacent_objs_)
        {
            if (to_check.checkColli(obj))
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
            std::cout << "\n##";
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