#pragma once
#include <TopoDS_Wire.hxx>
#include <Standard_Real.hxx>
#include <TopoDS_Shape.hxx>
#include <vector>
#include <TopoDS_Face.hxx>
#include <AIS_TextLabel.hxx>
#include "data.h"

void drawData(const std::vector<KBox> &buff, std::vector<TopoDS_Face>& vecset,
    std::vector<Handle(AIS_TextLabel)>& labs);




