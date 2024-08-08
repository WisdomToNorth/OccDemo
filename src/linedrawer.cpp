#include "linedrawer.h"
#include "global.h"

#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ViewCube.hxx>
#include <StdSelect_BRepOwner.hxx>
#include <TopoDS.hxx>
#include <AIS_ColoredShape.hxx>

#include "KLogger.h"
#include "gptools.h"
#include "shapetools.h"

namespace KDebugger
{

LineDrawer::LineDrawer(const gp_Pnt &pnt)
{
    pnt_list_.emplace_back(pnt);
    cur_mode_ = Mode::init;
}

void LineDrawer::handleExistPnt(const gp_Pnt &pt)
{
    // std::cout << " pnt_list_.size()" << pnt_list_.size() << std::endl;
    // std::cout << " viewmodel_vec_.size()" << viewmodel_vec_.size() << std::endl;
    size_t index = 0;
    auto it = pnt_list_.begin();
    while (it != pnt_list_.end())
    {
        if (it->IsEqual(pt, 0.001))
        {
            std::list<gp_Pnt> _;
            it++;
            _.splice(_.end(), pnt_list_, pnt_list_.begin(), it);
            pnt_list_.swap(_);
            break;
        }
        ++it;
        ++index;
        if (it == pnt_list_.end()) return;
        // 当前列表没有交点。相交的其他不相关的edge的点
        // 这里设计不太恰当
    }

    for (size_t i = index; i < viewmodel_vec_.size(); ++i)
    {
        G_Context->Remove(viewmodel_vec_[i], false);
    }
    auto mi = viewmodel_vec_.begin();
    std::advance(mi, index);
    std::vector<Handle(AIS_ColoredShape)> spliced(viewmodel_vec_.begin(), mi);
    viewmodel_vec_.swap(spliced);
    // std::cout << " ###pnt_list_.size()" << pnt_list_.size() << std::endl;
    // std::cout << " ###viewmodel_vec_.size()" << viewmodel_vec_.size() << std::endl;
}

void LineDrawer::checkDetectedObj(const gp_Pnt &new_pnt)
{
    if (!G_Context->HasDetected())
        return;
    else if (G_Context->DetectedInteractive()->IsKind(STANDARD_TYPE(AIS_ViewCube)))
        return;
    else
    {
        Handle(AIS_InteractiveObject) ais_shape = G_Context->DetectedInteractive();
        if (!ais_shape.IsNull())
        {
            Handle(AIS_Shape) ais_model = Handle(AIS_Shape)::DownCast(ais_shape);
            if (viewmodel_vec_.empty() ||                       // 当起始点直接就在图形内部时会出现此情况
                ais_model.get() == viewmodel_vec_.back().get()) // 画得很慢，上一笔画完的点被检测到
                return;
        }
        const auto &detect_owner = G_Context->DetectedOwner();
        Handle(StdSelect_BRepOwner) aBRepOwner = Handle(StdSelect_BRepOwner)::DownCast(detect_owner);
        if (aBRepOwner.IsNull()) return;
        const TopoDS_Shape &S = aBRepOwner->Shape();
        if (S.ShapeType() != TopAbs_EDGE) return;
        TopoDS_Edge shp = TopoDS::Edge(S);
        gp_Pnt to_check = getEdgeEndPts(shp).front();
        if (new_pnt.IsEqual(to_check, 0.2))
            handleExistPnt(to_check);
    }
}

std::string dir2Str(gp_Vec dir)
{
    std::stringbuf strbuf;
    Standard_OStream os(&strbuf);
    dir.DumpJson(os);

    return strbuf.str();
}

void LineDrawer::setCurDirection(const gp_Pnt &pnt)
{
    if (pnt_list_.size() < 1)
        direction_ = gp_Vec();
    else
        direction_ = gp_Vec(pnt_list_.back(), pnt); // .Normalized();

    std::cout << dir2Str(direction_) << std::endl;
}

bool LineDrawer::appendLine(const gp_Pnt &new_pnt,
                            double _angle, bool toggle)
{
    checkDetectedObj(new_pnt);
    // todo: not all new pnts append here
    const gp_Pnt last_pnt = pnt_list_.back();

    if (last_pnt.IsEqual(new_pnt, 0.01))
        return false;

    int togg = toggle == false ? 1 : -1;
    gp_Pnt mid = getAngledLineByTwoPts(direction_,
                                       last_pnt, new_pnt, _angle * M_PI / 180.0, togg);

    std::vector<gp_Pnt> pnts{mid, new_pnt};

    for (const auto &pt : pnts)
    {
        appendPnt(pt);
    }
    G_Context->Remove(temp_line_, false);
    return true;
}

void LineDrawer::appendPnt(const gp_Pnt &new_pnt)
{
    if (pnt_list_.back().IsEqual(new_pnt, 0.01)) return;

    const TopoDS_Edge &edge = getEdgeByTwoPts(pnt_list_.back(), new_pnt);

    // 下面两句有顺序依赖。设计得不好
    setCurDirection(new_pnt);
    pnt_list_.emplace_back(new_pnt);

    auto line = new AIS_ColoredShape(edge);
    line->SetWidth(3.0);

    G_Context->Display(line, true);
    viewmodel_vec_.emplace_back(line);
}

void LineDrawer::drawTempLine(const gp_Pnt &new_pnt,
                              double _angle, bool toggle)
{
    const gp_Pnt last_pnt = pnt_list_.back();
    if (last_pnt.IsEqual(new_pnt, 0.1)) return;

    int togg = toggle == false ? 1 : -1;
    gp_Pnt mid = getAngledLineByTwoPts(direction_,
                                       last_pnt, new_pnt, _angle * M_PI / 180.0, togg);

    std::vector<gp_Pnt> pnts{last_pnt, mid, new_pnt};

    TopoDS_Shape anEdge1 = getShapeByPts(pnts);

    G_Context->Remove(temp_line_, false);
    temp_line_ = new AIS_ColoredShape(anEdge1);

    G_Context->Display(temp_line_, 1, -1, true);
}

void LineDrawer::commitDraw(std::list<gp_Pnt> &pnts)
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
    for (auto &obj : viewmodel_vec_)
    {
        G_Context->Remove(obj, false);
    }
    G_Context->Remove(temp_line_, false);
    G_Context->UpdateCurrentViewer();
}
} // namespace KDebugger