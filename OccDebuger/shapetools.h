#pragma once
#include <Quantity_Color.hxx>

class Handle_AIS_Trihedron;

namespace OccTools
{
Handle_AIS_Trihedron createOriginTrihedron();
Quantity_Color getRandomColor();
}