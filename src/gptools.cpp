/****************************************************************************
** Copyright 2022 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/
#include "stadfx.h"
#include "gptools.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPoint>
#include <QString>
#include <QStringList>

#include "KPnt.h"
#include "BoundingBox.h"
#include "KLine.h"
#include "mathUtils.h"
#include "shapetools.h"

#define EPSILON 0.00000001

namespace OccTools
{

std::string trsfToStr(gp_Trsf trsf)
{
    std::stringbuf strbuf;
    Standard_OStream os(&strbuf);
    trsf.DumpJson(os);

    return strbuf.str();
}

QJsonArray trsfToJsonArr(gp_Trsf trsf)
{
    QJsonArray res;
    res.append(trsf.Value(1, 4));
    res.append(trsf.Value(2, 4));
    res.append(trsf.Value(3, 4));
    for (int r = 1; r <= 3; ++r)
    {
        for (int c = 1; c <= 3; ++c)
        {
            res.append(trsf.Value(r, c));
        }
    }
    return res;
}

std::string ptToStr(gp_Pnt pt, int decimal)
{
    std::stringstream  res;

    if (decimal != -1)
    {
        res << std::setiosflags(std::ios::fixed) <<
            std::setprecision(decimal) << pt.X() << ",";
        res << std::setiosflags(std::ios::fixed) <<
            std::setprecision(decimal) << pt.Y() << ",";
        res << std::setiosflags(std::ios::fixed) <<
            std::setprecision(decimal) << pt.Z();

    }
    else
    {
        res << pt.X() << ",";
        res << pt.Y() << ",";
        res << pt.Z();
    }
    return res.str();
}

QJsonArray ptToJsonArr(gp_Pnt pt)
{
    QJsonArray res;
    res.append(pt.X());
    res.append(pt.Y());
    res.append(pt.Z());

    return res;
}

std::string colorToStr(Quantity_Color color)
{
    std::stringbuf strbuf;
    Standard_OStream os(&strbuf);
    color.DumpJson(os);

    return strbuf.str();
}

QJsonArray colorToJsonArray(Quantity_Color color)
{
    QJsonArray color_arr;
    color_arr.append(color.Red());
    color_arr.append(color.Green());
    color_arr.append(color.Blue());
    return color_arr;
}

Quantity_Color jsonArrToColor(const QJsonArray& color_arr)
{
    return Quantity_Color(color_arr[0].toDouble(), color_arr[1].toDouble(),
        color_arr[2].toDouble(), Quantity_TOC_RGB);
}

gp_Pnt jsonArrToPt(QJsonArray arr)
{
    return gp_Pnt(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
}

gp_Dir jsonArr2Dir(QJsonArray arr)
{
    return gp_Dir(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
}

std::string dir2Str(gp_Dir dir)
{
    std::stringbuf strbuf;
    Standard_OStream os(&strbuf);
    dir.DumpJson(os);

    return strbuf.str();
}

gp_Pnt scalePt(const gp_Pnt& p1, double scale)
{
    return gp_Pnt(p1.X() * scale, p1.Y() * scale, p1.Z() * scale);
}

gp_Pnt minusPt(gp_Pnt p1, gp_Pnt p2)
{
    return gp_Pnt(p1.X() - p2.X(), p1.Y() - p2.Y(), p1.Z() - p2.Z());
}

gp_Pnt plusPt(gp_Pnt p1, gp_Pnt p2)
{
    return gp_Pnt(p1.X() + p2.X(), p1.Y() + p2.Y(), p1.Z() + p2.Z());
}

bool isEqualTrsf(const gp_Trsf& t1, const gp_Trsf& t2, double precision)
{
    NCollection_Mat4<float> theMatT1, theMatT2;
    t1.GetMat4(theMatT1);
    t2.GetMat4(theMatT2);

    for (int h = 0; h < 4; ++h)
    {
        for (int c = 0; c < 4; ++c)
        {
            auto v1 = theMatT1.GetValue(h, c);
            auto v2 = theMatT2.GetValue(h, c);
            if (abs(v1 - v2) > precision)
                return false;
        }
    }

    return true;
}

gp_Trsf strToTrsf(const std::string& str)
{
    gp_Trsf trsf;
    Standard_SStream is;
    int p = 1;
    is << str;
    trsf.InitFromJson(is, p);
    return trsf;
}

gp_Pnt pointProjLine(gp_Pnt pt, gp_Lin lin)
{
    gp_Vec V1(lin.Direction());
    gp_Pnt OR = lin.Location();
    gp_Vec V(OR, pt);
    Standard_Real dist = V1.Dot(V);
    gp_Pnt MyP = OR.Translated(dist * V1);
    Extrema_POnCurv MyPOnCurv(dist, MyP);
    return MyPOnCurv.Value();
}

gp_Pnt strToPnt(const QString& str)
{
    const QStringList& strlist = str.split(',');
    if (strlist.size() != 3)return gp_Pnt();
    gp_Pnt pnt(
        strlist[0].mid(1).toDouble(),
        strlist[1].mid(1).toDouble(),
        strlist[2].mid(1).left(strlist[2].indexOf(']') - 1).toDouble());
    return pnt;
}

gp_Trsf cpuRotZDirection(const gp_Ax2& ax2_0, const gp_Ax2& ax2_1)//从左到右变换
{
    gp_Trsf trsf_res, rotZ; gp_Trsf rotX;
    const auto& l1 = ax2_0.Direction();
    const auto& l2 = ax2_1.Direction();
    auto l_angle = l1.Angle(l2);
    if (abs(l_angle - 0) > EPSILON)
    {
        gp_Dir laserNormal;
        if (abs(l_angle - M_PI) < EPSILON)//两坐标系的Z轴平行反向，随便取一个方向作为旋转轴
        {
            laserNormal = ax2_1.XDirection();
        }
        else laserNormal = l1.Crossed(l2);//法向量

        gp_Ax1 l_ax(ax2_0.Location(), laserNormal);
        rotZ.SetRotation(l_ax, l_angle);
    }
    return rotZ;
}

gp_Trsf cpuRot(const gp_Ax2& ax2_0, const gp_Ax2& ax2_1)//从左到右变换
{
    gp_Trsf trsf_res, rotZ; gp_Trsf rotX;
    const auto& l1 = ax2_0.Direction();
    const auto& l2 = ax2_1.Direction();
    double l_angle = l1.Angle(l2);
    if (abs(l_angle - 0) > EPSILON)
    {
        gp_Dir laserNormal;
        if (abs(l_angle - M_PI) < EPSILON)//两坐标系的Z轴平行反向，随便取一个方向作为旋转轴
        {
            laserNormal = ax2_1.XDirection();
        }

        else laserNormal = l1.Crossed(l2);//法向量

        gp_Ax1 l_ax(ax2_0.Location(), laserNormal);
        rotZ.SetRotation(l_ax, l_angle);
    }
    auto ax2_2 = ax2_0.Transformed(rotZ);//已经对齐Z轴的坐标系
    const auto& l3 = ax2_1.XDirection();
    const auto& l4 = ax2_2.XDirection();
    auto l_angle2 = l3.Angle(l4);
    if (abs(l_angle2 - 0) > EPSILON)
    {
        gp_Dir laserNormal2;
        if (abs(l_angle2 - M_PI) < EPSILON)//两坐标系的Z轴平行反向，随便取一个方向作为旋转轴
        {
            laserNormal2 = ax2_1.Direction();//这里取Z方向
        }

        else laserNormal2 = l3.Crossed(l4);//计算法向量。主要是因为法向量有方向。

        gp_Ax1 l_ax2(ax2_0.Location(), laserNormal2);
        rotX.SetRotation(l_ax2, -l_angle2);
    }

    trsf_res = rotX * rotZ;
    return trsf_res;
}

gp_Trsf cpuTrsf(const gp_Ax2& ax2_0, const gp_Ax2& ax2_1)//第一个参数是目标，第二个参数是当前
{
    gp_Trsf trsf_res, trsf_trans, trsf_rot;
    trsf_trans.SetTranslation(ax2_0.Location(), ax2_1.Location());
    trsf_rot = cpuRot(ax2_0, ax2_1);
    trsf_res = trsf_trans * trsf_rot;
    return trsf_res;
}

gp_Vec moveOnDir(gp_Dir aDir, double aLength)
{
    gp_Pnt p(aDir.XYZ().Normalized());
    return gp_Vec(gp_Pnt(0, 0, 0), scalePt(p, aLength));
}

gp_Pnt newPointOnDir(gp_Pnt beginPoint, gp_Dir dir, double length)
{
    gp_Pnt p(dir.XYZ().Normalized());;
    return plusPt(beginPoint, scalePt(p, length));
}

gp_Pnt getCenterOfPnts(const std::vector<gp_Pnt>& ori_pts)
{
    gp_Pnt center_pnt;
    double x = 0.0, y = 0.0, z = 0.0;
    for (const auto& pnt : ori_pts)
    {
        x = x + pnt.X();
        y = y + pnt.Y();
        z = z + pnt.Z();
    }
    size_t cnt = ori_pts.size();
    center_pnt.SetX(x / cnt);
    center_pnt.SetY(y / cnt);
    center_pnt.SetZ(z / cnt);
    return center_pnt;
}

gp_Dir getNormalByThreePnts(const std::vector<gp_Pnt>& ori_pts)
{
    if (ori_pts.size() != 3)return gp_Dir();

    gp_Dir d1(gp_Vec(ori_pts[0], ori_pts[1]));
    gp_Dir d2(gp_Vec(ori_pts[1], ori_pts[2]));
    return d1.Crossed(d2);
}

TopoDS_Edge getEdgeByTwoPts(const gp_Pnt& p1, const gp_Pnt& p2)
{
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(p1, p2);
    return BRepBuilderAPI_MakeEdge(aSegment1);
}

TopoDS_Shape getShapeByPts(const std::vector<gp_Pnt>& pnts)
{
    TopoDS_Compound t_compound;
    BRep_Builder t_build_tool;
    t_build_tool.MakeCompound(t_compound);

    for (size_t i = 0; i < pnts.size() - 1; ++i)
    {
        t_build_tool.Add(t_compound, getCircleFromPt(pnts[i], 0.02));
        if (pnts[i].IsEqual(pnts[i + 1], 0.01))
            continue;
        t_build_tool.Add(t_compound, getEdgeByTwoPts(pnts[i], pnts[i + 1]));
    }

    return t_compound;
}

//返回离excludePt最远的点
gp_Pnt getEndPtEdge(TopoDS_Shape shp, gp_Pnt excludePt)
{
    gp_Pnt pt1_1, pt1_2;
    if (shp.ShapeType() == TopAbs_EDGE)
    {
        Standard_Real first, last;
        Handle(Geom_Curve) aCurve1 = BRep_Tool::Curve(TopoDS::Edge(shp), first, last);
        pt1_1 = aCurve1->Value(first);
        pt1_2 = aCurve1->Value(last);
    }
    else
    {
        TopTools_IndexedMapOfShape edgemap;
        TopExp::MapShapes(shp, TopAbs_EDGE, edgemap);
        Standard_Real first, last;
        Handle(Geom_Curve) aCurve1 = BRep_Tool::Curve(TopoDS::Edge(edgemap(1)), first, last);
        pt1_1 = aCurve1->Value(first);

        Standard_Integer idx = edgemap.Extent();
        aCurve1 = BRep_Tool::Curve(TopoDS::Edge(edgemap(idx)), first, last);
        pt1_2 = aCurve1->Value(last);
    }

    if (excludePt.Distance(pt1_1) < excludePt.Distance(pt1_2))
        return pt1_2;
    else
        return pt1_1;
}

//if curve is not line, may be have several result;
//the input is line here, so only have one result
std::vector<gp_Pnt2d> getLineCross(const gp_Lin2d& aLine1, const gp_Lin2d& aLine2)
{
    std::vector<gp_Pnt2d> res;
    IntAna2d_AnaIntersection aIntAna;
    aIntAna.Perform(aLine1, aLine2);
    if (aIntAna.IsDone())
    {
        for (size_t i = 1; i <= aIntAna.NbPoints(); ++i)
        {
            const IntAna2d_IntPoint& pt = aIntAna.Point(i);
            res.emplace_back(gp_Pnt2d(pt.Value().X(), pt.Value().Y()));
        }
    }
    return res;
}

double getAngle(gp_Pnt ori, gp_Pnt p1, gp_Pnt p2)
{
    gp_Vec v1(ori, p1);
    gp_Vec v2(ori, p2);
    return v1.AngleWithRef(v2, gp_Vec(0, 0, 1));
}

int sameDir(gp_Vec v1, gp_Vec v2)
{
    const double& res = std::abs(v1.AngleWithRef(v2, gp_Vec(0, 0, 1)));
    if (fEqual(res, 0))return 1;
    else if (fEqual(res, M_PI))return -1;
    else return 0;
}

double symbol(double num)
{
    if (num > 0)return 1.0;
    else if (num < 0)return -1.0;
    else return 0;
}

gp_Pnt getAngledLineByTwoPts(const gp_Vec& last_vec, const gp_Pnt& pA,
    const gp_Pnt& pB, const double& _angle, int _toggle)
{
    if (last_vec.IsEqual(gp_Vec(), 0.01, 0.01))//rand direction
    {
        return getAngledLineByTwoPtsRandDir(last_vec, pA, pB, _angle, _toggle);
    }
    else if (last_vec.Dot(gp_Vec(1, 0, 0)) == 0 || last_vec.Dot(gp_Vec(0, 1, 0)) == 0)
    {
        return getAngledLineByTwoPtsOrth(last_vec, pA, pB, _angle, _toggle);
    }
    else
    {
        //分解为两个正交方向
        gp_Vec dir_1 = gp_Vec(last_vec.X(), 0, 0);
        gp_Vec dir_2 = gp_Vec(0, last_vec.Y(), 0);
        if (last_vec.AngleWithRef(dir_1, gp_Vec(0, 0, 1)) < 0)//确保第一个向量与angle>0的方向一致
        {
            gp_Vec temp = dir_1; dir_1 = dir_2; dir_2 = temp;
        }

        double angle = last_vec.AngleWithRef(gp_Vec(pA, pB), gp_Vec(0, 0, 1));

        if (angle > 0 && angle < 90.0 * M_PI / 180.0)
        {
            return getAngledLineByTwoPtsOrth(dir_1, pA, pB, _angle, _toggle);
        }
        else if (angle > -90 * M_PI / 180.0 && angle < 0)//angle < 0
        {
            return getAngledLineByTwoPtsOrth(dir_2, pA, pB, _angle, _toggle);
        }
        else//need reverse
        {
            return getAngledLineByTwoPtsReverse(last_vec, pA, pB, _toggle);
        }
    }
}

gp_Pnt getAngledLineByTwoPtsReverse(const gp_Vec& last_vec, const gp_Pnt& pA,
    const gp_Pnt& pB, int _toggle)
{
    double angle = last_vec.AngleWithRef(gp_Vec(pA, pB), gp_Vec(0, 0, 1));
    double aux_angle = angle + 45 * M_PI / 180.0;
    int is_left = symbol(angle);
    gp_Lin lin = gp_Lin(pA, last_vec);
    double dist = lin.Distance(pB);
    KDebugger::KPt pD;
    pD.x = pA.X() - is_left * dist * last_vec.Normalized().Y();
    pD.y = pA.Y() + is_left * dist * last_vec.Normalized().X();
    return gp_Pnt(pD.x, pD.y, 0.0);
}

gp_Pnt getAngledLineByTwoPtsIncline(const gp_Vec& last_vec, const gp_Pnt& pA,
    const gp_Pnt& pB, const double& _angle, int _toggle)
{

    using namespace KDebugger;

    KBoundingBox _box = KBoundingBox(KPt(pA), KPt(pB));

    double dx = std::abs(pA.X() - pB.X());
    double dy = std::abs(pA.Y() - pB.Y());
    KPt pC, pD;

    double angle = last_vec.AngleWithRef(gp_Vec(pA, pB), gp_Vec(0, 0, 1));
    double aux_angle = angle + 45 * M_PI / 180.0;

    if ((sameDir(last_vec, gp_Vec(1, 1, 0)) == 1 && _toggle == 1) ||
        (sameDir(last_vec, gp_Vec(1, 1, 0)) == -1 && _toggle == -1))
    {
        if (Tan(aux_angle) < 0) pC = _box.ru_;
        else if (Tan(angle) > 0)pC = _box.rb_;
        else pC = _box.lu_;

        if (dx < dy)
        {
            pD.x = pC.x;
            if (Tan(aux_angle) < 0)pD.y = pC.y - Tan(_angle) * dx;
            else pD.y = pC.y + Tan(_angle) * dx;
        }
        else
        {
            pD.y = pC.y;
            if (Tan(aux_angle) < 0)pD.x = pC.x - Tan(_angle) * dy;
            else pD.x = pC.x + Tan(_angle) * dy;
        }
    }
    else if ((sameDir(last_vec, gp_Vec(1, 1, 0)) == 1 && _toggle == -1) ||
        (sameDir(last_vec, gp_Vec(1, 1, 0)) == -1 && _toggle == 1))
    {
        if (Tan(aux_angle) < 0) pC = _box.lb_;
        else if (Tan(angle) > 0)pC = _box.lu_;
        else pC = _box.rb_;

        if (dx < dy)
        {
            pD.x = pC.x;
            if (Tan(aux_angle) < 0)pD.y = pC.y + Tan(_angle) * dx;
            else pD.y = pC.y - Tan(_angle) * dx;
        }
        else
        {
            pD.y = pC.y;
            if (Tan(aux_angle) < 0)pD.x = pC.x + Tan(_angle) * dy;
            else pD.x = pC.x - Tan(_angle) * dy;
        }
    }
    else if ((sameDir(last_vec, gp_Vec(1, -1, 0)) == 1 && _toggle == 1) ||
        (sameDir(last_vec, gp_Vec(1, -1, 0)) == -1 && _toggle == -1))
    {
        if (Tan(aux_angle) < 0) pC = _box.rb_;
        else if (Tan(angle) > 0)pC = _box.lb_;
        else pC = _box.ru_;

        if (dx < dy)
        {
            pD.x = pC.x;
            if (Tan(aux_angle) < 0)pD.y = pC.y + Tan(_angle) * dx;
            else pD.y = pC.y - Tan(_angle) * dx;
        }
        else
        {
            pD.y = pC.y;
            if (Tan(aux_angle) < 0)pD.x = pC.x - Tan(_angle) * dy;
            else pD.x = pC.x + Tan(_angle) * dy;
        }
    }
    else if ((sameDir(last_vec, gp_Vec(1, -1, 0)) == 1 && _toggle == -1) ||
        (sameDir(last_vec, gp_Vec(1, -1, 0)) == -1 && _toggle == 1))
    {
        if (Tan(aux_angle) < 0) pC = _box.lu_;
        else if (Tan(angle) > 0)pC = _box.ru_;
        else pC = _box.lb_;

        if (dx < dy)
        {
            pD.x = pC.x;
            if (Tan(aux_angle) < 0)pD.y = pC.y - Tan(_angle) * dx;
            else pD.y = pC.y + Tan(_angle) * dx;
        }
        else
        {
            pD.y = pC.y;
            if (Tan(aux_angle) < 0)pD.x = pC.x + Tan(_angle) * dy;
            else pD.x = pC.x - Tan(_angle) * dy;
        }
    }

    return gp_Pnt(pD.x, pD.y, 0.0);
}

gp_Pnt getAngledLineByTwoPtsRandDir(const gp_Vec& last_vec, const gp_Pnt& pA,
    const gp_Pnt& pB, const double& _angle, int _toggle)
{
    using namespace KDebugger;

    double dx = std::abs(pA.X() - pB.X());
    double dy = std::abs(pA.Y() - pB.Y());
    KPt pC, pD;

    pC.x = ((dx > dy && _toggle == 1) || (dx <= dy && _toggle == -1)) ? pB.X() : pA.X();
    pC.y = ((dx > dy && _toggle == 1) || (dx <= dy && _toggle == -1)) ? pA.Y() : pB.Y();

    double angle = gp_Vec(1, 0, 0).AngleWithRef(gp_Vec(pA, pB), gp_Vec(0, 0, 1));
    double temp_angle = angle + 45.0 * M_PI / 180.0;
    if (Tan(temp_angle) > 0)//等效于dx>dy
    {
        pD.x = pC.x - symbol(Sin(temp_angle)) * Tan(_angle) * dy * _toggle;
        pD.y = pC.y;
    }
    else
    {
        pD.x = pC.x;
        pD.y = pC.y - symbol(Sin(temp_angle)) * Tan(_angle) * dx * _toggle;
    }
    return gp_Pnt(pD.x, pD.y, 0.0);
}

gp_Pnt getAngledLineByTwoPtsOrth(const gp_Vec& last_vec, const gp_Pnt& pA,
    const gp_Pnt& pB, const double& _angle, int _toggle)
{

    using namespace KDebugger;
    //std::cout << "last dir Orth: " << dir2Str(last_vec) << std::endl;
    KBoundingBox _box = KBoundingBox(KPt(pA), KPt(pB));

    double dx = std::abs(pA.X() - pB.X());
    double dy = std::abs(pA.Y() - pB.Y());
    KPt pC, pD;

    double angle = last_vec.AngleWithRef(gp_Vec(pA, pB), gp_Vec(0, 0, 1));

    if ((sameDir(last_vec, gp_Vec(1, 0, 0)) == 1 && _toggle == 1) ||
        (sameDir(last_vec, gp_Vec(1, 0, 0)) == -1 && _toggle == -1))
    {

        pC.x = _box.right();
        pC.y = Tan(angle) > 0/*一四象限*/ ? _box.bottom() : _box.top();

        if (dx < dy)
        {
            pD.x = pC.x;
            pD.y = pC.y + symbol(Tan(angle)) * Tan(_angle) * dx;
        }
        else//dx>dy
        {
            pD.x = pC.x - Tan(_angle) * dy;
            pD.y = pC.y;
        }
    }
    else if ((sameDir(last_vec, gp_Vec(1, 0, 0)) == 1 && _toggle == -1) ||
        (sameDir(last_vec, gp_Vec(1, 0, 0)) == -1 && _toggle == 1))
    {
        pC.x = _box.left();
        pC.y = Tan(angle) > 0 ? _box.top() : _box.bottom();

        if (dx < dy)
        {
            pD.x = pC.x;
            pD.y = pC.y - symbol(Tan(angle)) * Tan(_angle) * dx;
        }
        else//dx>dy
        {
            pD.x = pC.x + Tan(_angle) * dy;
            pD.y = pC.y;
        }
    }
    else if ((sameDir(last_vec, gp_Vec(0, 1, 0)) == 1 && _toggle == 1) ||
        (sameDir(last_vec, gp_Vec(0, 1, 0)) == -1 && _toggle == -1))
    {
        pC.y = _box.top();
        pC.x = Tan(angle) > 0 ? _box.right() : _box.left();

        if (dx < dy)
        {
            pD.x = pC.x;
            pD.y = pC.y - Tan(_angle) * dx;
        }
        else//dx>dy
        {
            pD.x = pC.x - symbol(Tan(angle)) * Tan(_angle) * dy;
            pD.y = pC.y;
        }
    }
    else if ((sameDir(last_vec, gp_Vec(0, 1, 0)) == 1 && _toggle == -1) ||
        (sameDir(last_vec, gp_Vec(0, 1, 0)) == -1 && _toggle == 1))
    {
        pC.y = _box.bottom();
        pC.x = Tan(angle) > 0 ? _box.left() : _box.right();

        if (dx < dy)
        {
            pD.x = pC.x;
            pD.y = pC.y + Tan(_angle) * dx;
        }
        else//dx>dy
        {
            pD.x = pC.x + symbol(Tan(angle)) * Tan(_angle) * dy;
            pD.y = pC.y;
        }
    }

    return gp_Pnt(pD.x, pD.y, 0.0);
}
}