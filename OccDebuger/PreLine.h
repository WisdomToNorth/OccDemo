#pragma once

#include <gp_Pnt.hxx>
#include <AIS_ColoredShape.hxx>

#include <vector>
#include <list>

#include "BoundingBox.h"
#include "K_box.h"

namespace KDebugger
{

class PrePline
{
public:
    PrePline(const std::list<gp_Pnt>& res);

    void hide();
    void updateColor(Quantity_Color color = Quantity_NOC_ALICEBLUE);

    void drawRawPnts(Quantity_Color color = Quantity_NOC_BLACK);
    void drawPnts(Quantity_Color color = Quantity_NOC_BLACK);
    void reGenerate(const std::vector<KBox>& context_info);
    void normlizeSegment();
private:
    void caculateEnviroment(const std::vector<KBox>& context_info);
    void updateAABB();
    void printAllPts();

private:
    std::list<gp_Pnt> raw_pnts_;
    std::list<gp_Pnt> new_pnts_;
    std::vector<Handle(AIS_ColoredShape)> viewmodel_vec_;
    std::vector<KBox> adjacent_objs_;
    KBoundingBox aabbox_;
};

}