/****************************************************************************
** Copyright 2022 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/

#include "gptools.h"

#include <Quantity_Color.hxx>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <QPoint>
#include <QString>
#include <QStringList>

#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Dir.hxx>
#include <Geom_BSplineCurve.hxx>


#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>

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

TopoDS_Edge drawLineByTwoPts(const gp_Pnt& p1, const gp_Pnt& p2)
{
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(p1, p2);
    return BRepBuilderAPI_MakeEdge(aSegment1);
}
}