#pragma once
#include <iostream>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <thread>

#include <AIS_TextLabel.hxx>
#include <TopoDS_Face.hxx>

#include "KPnt.h"
#include "KLine.h"

class AIS_Shape;

namespace KDebugger
{

class KBoundingBox;
class KBox
{
public:

    KBox(double x, double y, double sizex, double sizey, int type);
    ~KBox();
    bool isOut(const KBox& rhs)const;
    bool outBox(const KPt& pt)const;
    bool outBoxWithSpacing(const KPt& pt)const;
    bool isCrossWithKLine(const KLine& line)const;
    bool isCrossWithKLineWithSpace(const KLine& line)const;
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
    KBoundingBox getSpaceBoundingbox()const;
    void setVal(const int& val) { val_ = val; }
    void show();

private:
    void drawElips();
    void drawBox();
    void drawSpacingBox();
    Handle(AIS_TextLabel) getText();

protected:
    KPt center_;
    double size_x_;
    double size_y_;
    double space_ = 0.15;
    int val_ = -1;//label
    enum class ObjType
    {
        Elips, Box
    };
    ObjType type_;

};



}