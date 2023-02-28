/****************************************************************************
** Copyright 2022 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/

#pragma once
#ifndef _GPHANDLE_H_
#define _GPHANDLE_H_
#pragma warning(disable : 26451)
#pragma warning(disable : 4267) 


#include <iomanip> 

#include <vector>
#include <Precision.hxx>

class QString;
class QJsonArray;
class QJsonObject;
class QPoint;
class Quantity_Color;
class gp_Pnt;
class gp_Dir;
class gp_Pln;
class gp_Trsf;
class TopoDS_Edge;
class gp_Lin;
class gp_Vec;
class gp_Ax2;
class gp_Pnt2d;
class gp_Lin2d;

namespace OccTools
{

std::string trsfToStr(gp_Trsf trsf);

QJsonArray trsfToJsonArr(gp_Trsf trsf);

std::string ptToStr(gp_Pnt pt, int decimal = -1);

QJsonArray ptToJsonArr(gp_Pnt pt);

std::string colorToStr(Quantity_Color color);

QJsonArray colorToJsonArray(Quantity_Color color);

Quantity_Color jsonArrToColor(const QJsonArray& color_arr);

gp_Pnt jsonArrToPt(QJsonArray arr);

gp_Dir jsonArr2Dir(QJsonArray arr);

std::string dir2Str(gp_Dir dir);

gp_Pnt scalePt(const gp_Pnt& p1, double scale);

gp_Pnt minusPt(gp_Pnt p1, gp_Pnt p2);

gp_Pnt plusPt(gp_Pnt p1, gp_Pnt p2);

bool isEqualTrsf(const gp_Trsf& t1, const gp_Trsf& t2,
    double precision = Precision::Confusion());

gp_Pnt strToPnt(const QString& str);

gp_Trsf strToTrsf(const std::string& str);

gp_Trsf cpuRotZDirection(const gp_Ax2& ax2_0, const gp_Ax2& ax2_1);

gp_Trsf cpuRot(const gp_Ax2& ax2_0, const gp_Ax2& ax2_1);
gp_Pnt pointProjLine(gp_Pnt pt, gp_Lin lin);
gp_Trsf cpuTrsf(const gp_Ax2& ax2_0, const gp_Ax2& ax2_1);

gp_Vec moveOnDir(gp_Dir aDir, double aLength);

gp_Pnt newPointOnDir(gp_Pnt beginPoint, gp_Dir dir, double length);

gp_Pnt getCenterOfPnts(const std::vector<gp_Pnt>& ori_pts);

gp_Dir getNormalByThreePnts(const std::vector<gp_Pnt>& ori_pts);

TopoDS_Edge drawLineByTwoPts(const gp_Pnt& p1, const gp_Pnt& p2);
std::vector<TopoDS_Edge> drawAngledLineByTwoPts(const gp_Pnt& p1,
    const gp_Pnt& p2);

//if curve is not line, may be have several result;
//the input is line here, so only have one result
std::vector<gp_Pnt2d> getLineCross(const gp_Lin2d& aLine1, const gp_Lin2d& aLine2);
}
#endif