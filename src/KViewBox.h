#pragma once

#include "KBox.h"

namespace KDebugger
{

class KViewBox : public KBox
{
public:
    KViewBox(double x, double y, double size = 0.1);
    ~KViewBox();

    void tempshow();
    void temphide();

private:
    Handle(AIS_Shape) temp_obj_ = nullptr;
};

} // namespace KDebugger