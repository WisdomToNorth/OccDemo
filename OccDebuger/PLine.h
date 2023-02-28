#pragma once

#include <gp_Pnt.hxx>
#include <AIS_ColoredShape.hxx>

#include <vector>
#include <list>

namespace KDebugger
{

class PrePline
{
public:
    PrePline(const std::list<gp_Pnt>& res) :raw_pnts_(res) {}

    void draw();

    void hide();
    void drawRawPnts();

private:
    std::list<gp_Pnt> raw_pnts_;
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec_;

};

}