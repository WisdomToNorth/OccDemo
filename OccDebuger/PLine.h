#pragma once

#include <gp_Pnt.hxx>
#include <AIS_ColoredShape.hxx>

#include <vector>

namespace KDebugger
{

class PLine
{
public:
    PLine(const std::vector<gp_Pnt>& res) :raw_pnts_(res) { }

    void draw();

    void hide();
    void drawRawPnts();

private:
    std::vector<gp_Pnt> raw_pnts_;
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec_;

};

}