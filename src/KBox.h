﻿#pragma once
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
struct KBoundingBox;
class KBox
{
public:
    KBox(double x, double y, double sizex, double sizey, int type);

    ~KBox();
    bool isOut(const KBox &rhs) const;
    bool outBox(const KPt &pt) const;
    bool outBoxWithSpacing(const KPt &pt) const;
    bool isCrossWithKLine(const KLine &line) const;
    bool isCrossWithKLineWithSpace(const KLine &line) const;
    void mergeTest(const KBox &)
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    void mergeTest(std::vector<KBox> &boxs)
    {
        // std::cout << "\nmerge: ";
        // for (const auto& box : boxs)
        //{
        //     std::cout << box.val_ << ' ';
        // }
        // std::cout << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    const KPt &getPt() const
    {
        return center_;
    }
    KPt leftBottom() const;
    KPt rightUp() const;
    KPt rightBottom() const;
    KPt leftUp() const;

    KBoundingBox getBoundingbox() const;
    KBoundingBox getSpaceBoundingbox() const;
    void setVal(const int &val)
    {
        val_ = val;
    }
    void show();

    int val_ = -1; // label
private:
    void drawElips();
    void drawBox();
    void drawSpacingBox();
    Handle(AIS_TextLabel) getText();

protected:
    KPt center_;
    double size_x_;
    double size_y_;
    double space_ = 0.0; // 15;

    enum class ObjType
    {
        Elips,
        Box
    };
    ObjType type_;
};
} // namespace KDebugger