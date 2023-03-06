#include "PreLine.h"

#include "gptools.h"

#include <TopoDS_Edge.hxx>

#include "global.h"
#include "KLine.h"
#include "KAngleMultiLine.h"
#include "shapetools.h"
#include "stadfx.h"

#include "K_box.h"
#include <GC_MakeArcOfCircle.hxx>

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

void PrePline::postProcessLine()
{

}

void PrePline::drawMultiLine(Quantity_Color)
{
    auto cur = multiline_list_.begin();
    while (cur != multiline_list_.end())
    {
        if (cur->resSize())
        {
            auto color = OccTools::getRandomColor();
            const std::vector<TopoDS_Edge>& edges = cur->getEdge();

            for (const auto& edge : edges)
            {
                Handle(AIS_ColoredShape) line = new AIS_ColoredShape(edge);
                line->SetWidth(3.0);
                line->SetColor(color);
                G_Context->Display(line, false);
                viewmodel_vec_.push_back(line);
            }
            cur++;
        }
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

void PrePline::updatePreviousColor(Quantity_Color color)
{
    updateColor();
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec;
    viewmodel_vec_.swap(viewmodel_vec_);
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
    opt_open_ = opt_open_ == true ? false : true;
    std::cout << "optimize is " << opt_open_ << std::endl;
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

    std::list<KAngleMultiLine>temp_newpnt;
    multiline_list_.swap(temp_newpnt);

    auto cur_vertax = raw_pnts_.begin();
    // multiline_list_.emplace_back(*cur_vertax);

    auto cur_next = raw_pnts_.begin();
    cur_next++;

    KAngleMultiLine last_check(*cur_vertax, *cur_next);
    while (cur_next != raw_pnts_.end())
    {
        //for debug, break endless loop
        if (GCheckCpuMode())return;

        bool colli = false;
        KAngleMultiLine cur_check(*cur_vertax, *cur_next);
        cur_check.cpuAvailable(45);

        for (const KBox& obj : adjacent_objs_)
        {
            if ((!obj.outBoxWithSpacing(KPt(cur_vertax->X(), cur_vertax->Y()))) ||
                (!obj.outBoxWithSpacing(KPt(cur_next->X(), cur_next->Y()))))
            {
                std::cout << "pnt in box! unhandled!\n";
                return;//TODO: draw in box is unhandled currently
            }


            if (cur_check.checkColli(obj))
                //碰撞，或者与上一条line呈小夹角，则记录当前的上一个点。
            {
                colli = true;
                break;
            }
        }
        if (!colli)//
        {
            cur_next++;
            last_check = cur_check;
        }
        else//collision
        {
            auto temp = cur_next;
            temp--;
            cur_vertax = temp;

            multiline_list_.push_back(last_check);

            have_tried_ = false;

        }
    }
    multiline_list_.emplace_back(last_check);

    //if (opt_open_)
    //{
    //    std::cout << "optimize is on\n";
    //    postProcessLine();
    //}

    this->updatePreviousColor();//colored previous to gray
    this->drawMultiLine();
}
}