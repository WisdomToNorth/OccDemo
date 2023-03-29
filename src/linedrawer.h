#pragma once

#include <unordered_map>
#include <vector>
#include <gp_Pnt.hxx>
#include <AIS_ColoredShape.hxx>
#include <list>

namespace KDebugger
{

class LineDrawer
{
private:
    enum class Mode
    {
        init,
        getnext,
        exit
    };
    //init(wait first),wait sec,confirm
    //invalid,valid,exitcommand
    std::unordered_map<Mode, std::vector<Mode>> state_map_
    {
        {Mode::init,{Mode::init,Mode::getnext,Mode::exit}},
        {Mode::getnext,{Mode::getnext,Mode::getnext,Mode::exit}},
        {Mode::exit,{Mode::init,Mode::exit,Mode::exit}}
    };

public:
    LineDrawer(const gp_Pnt& pnt);
    ~LineDrawer() {}

    void commitDraw(std::list<gp_Pnt>& pnts);
    void checkDetectedObj(const gp_Pnt& new_pnt);
    void cancelDraw();
    void removeTempViewModel();
    bool appendLine(const gp_Pnt& pnt, double _angle, bool toggle);
    void drawTempLine(const gp_Pnt& new_pnt, double _angle, bool toggle);
    void handleExistPnt(const gp_Pnt& pt);

private:
    void setCurDirection(const std::vector<gp_Pnt>& pnts);

private:

    Mode cur_mode_;
    std::list<gp_Pnt> pnt_list_;
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec_;
    Handle(AIS_ColoredShape) temp_line_;
    gp_Vec direction_;
};

}