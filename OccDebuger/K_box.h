#pragma once
#include <iostream>
#include <Windows.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <thread>

#include <AIS_TextLabel.hxx>
#include <TopoDS_Face.hxx>

#include "K_Pnt.h"

class AIS_Shape;

namespace KDebugger
{
class KLine;
class KBoundingBox;
class KBox
{
public:
    KBox(double x, double y, double sizex, double sizey, int type);

    bool isOut(const KBox& rhs)const;
    bool isCrossWith(const KLine& line)const;
    bool isCrossWithVal(KLine line);
    void mergeTest(const KBox&)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    void mergeTest(std::vector<KBox>&)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    const KPt& getPt()const { return center_; }
    KBoundingBox getBoundingbox()const;
    void setVal(const int& val) { val_ = val; }
    void show();

private:
    void drawElips();
    void drawBox();
    Handle(AIS_TextLabel) getText();
private:
    KPt center_;
    double size_x_;
    double size_y_;
    int val_ = -1;//label
    enum class ObjType
    {
        Elips, Box
    };
    ObjType type_;
};



}