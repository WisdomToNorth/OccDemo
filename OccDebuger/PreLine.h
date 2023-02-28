#pragma once

#include <gp_Pnt.hxx>
#include <AIS_ColoredShape.hxx>

#include <vector>
#include <list>

#include "BoundingBox.h"

namespace KDebugger
{

class PrePline
{
public:
    PrePline(const std::list<gp_Pnt>& res);

    void draw();

    void hide();
    void drawRawPnts();
    void reGenerate(const std::vector<KBox>& context_info);
private:
    void updateAABB();

private:
    std::list<gp_Pnt> raw_pnts_;
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec_;
    KBoundingBox aabbox_;
};

}