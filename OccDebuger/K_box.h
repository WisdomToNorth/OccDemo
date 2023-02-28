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
class KBox
{
public:
    KBox(double x, double y, double sizex, double sizey, int type);
    bool isOut(const KBox& rhs)
    {
        double cx = (size_x + rhs.size_x) * 0.5;
        double cy = (size_y + rhs.size_y) * 0.5;

        if (abs(rhs.center_.x - center_.x) > cx || abs(rhs.center_.y - center_.y) > cy)
            return true;
        else return false;
    }
    void mergeTest(const KBox&)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    void mergeTest(std::vector<KBox>&)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    double X()const { return center_.x; }
    double Y()const { return center_.y; }
    const KPt& getPt()const { return center_; }
    void setVal(const int& val) { val_ = val; }
    void show();

private:
    void drawElips();
    void drawBox();
private:
    KPt center_;
    double size_x;
    double size_y;
    int val_ = -1;//label
    enum class ObjType
    {
        Elips, Box
    };
    ObjType type_;
};



}