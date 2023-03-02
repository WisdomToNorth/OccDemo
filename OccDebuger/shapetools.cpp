#include "shapetools.h"


#include <AIS_Trihedron.hxx>
#include <Geom_Axis2Placement.hxx>

#include "global.h"

namespace OccTools
{
Handle_AIS_Trihedron createOriginTrihedron()
{
    Handle_Geom_Axis2Placement axis = new Geom_Axis2Placement(gp::XOY());
    Handle_AIS_Trihedron aisTrihedron = new AIS_Trihedron(axis);
    aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
    aisTrihedron->SetDrawArrows(false);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
    aisTrihedron->SetLabel(Prs3d_DP_XAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_YAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "");
    aisTrihedron->SetSize(60);
    aisTrihedron->SetTransformPersistence(
        new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location())
    );
    aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
    aisTrihedron->SetInfiniteState(true);
    return aisTrihedron;
}

Quantity_Color getRandomColor()
{
    std::uniform_int_distribution<int>c(0, 5);
    int cur = c(KDebugger::G_Random_Engine);
    if (cur == 0)return Quantity_NOC_BLUE;
    else if (cur == 1)return Quantity_NOC_RED;
    else if (cur == 2)return Quantity_NOC_GREEN;
    else if (cur == 3)return Quantity_NOC_BROWN;
    else if (cur == 4)return Quantity_NOC_ORANGE;
    else return Quantity_NOC_BLUE3;

}
}