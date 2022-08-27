#include "public.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>

#include <gp_Elips.hxx>
#include <Geom2d_Ellipse.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Face.hxx>
#include <AIS_TextLabel.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>

#include <qstring.h>
namespace DataUtil
{
void drawData(const std::vector<KBox>& buff, std::vector<TopoDS_Face>& vecset,
    std::vector<Handle(AIS_TextLabel)>& labs)
{
    int testsize = buff.size();
    int cnt = 0;
    gp_Dir dir = gp_Dir(0, 0, 1);
    gp_Dir dirx = gp_Dir(1, 0, 0);
    for (const auto& box : buff)
    {
        gp_Pnt loc(box.X(), box.Y(), 0);
        gp_Elips ge(gp_Ax2(loc, dir, dirx), box.size_x*0.5, box.size_y * 0.5);
        TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(ge);
        BRepBuilderAPI_MakeWire WW(e2);
        TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(WW);
        vecset.emplace_back(myFaceProfile);

        if (testsize < 1000)
        {
            gp_Pnt cur(box.X(), box.Y(), 0);
            Handle(AIS_TextLabel) text = new AIS_TextLabel();
            text->SetPosition(cur);
            QString str;
            str += QString::number(cnt);
                //+ '#' + QString::number(box.size_x)
                //+ '#'+QString::number(box.size_y);
            text->SetText(str.toStdString().c_str());
            labs.emplace_back(text);
            ++cnt;
        }
    }
}

void generateTestData(std::vector<KBox>& buffer, int testrow, int testcol, int distance)
{
    if (testcol == 0)testcol = testrow;
    std::default_random_engine e;
    std::uniform_real_distribution<double> sizeu(0.8, 1);
    std::uniform_real_distribution<double> v(0.5, 1);
    e.seed(1);
    for (int i = 0; i < testcol; ++i)
    {
        for (int j = 0; j < testrow; ++j)
        {
            double loc_random = v(e);
            double stx = i * distance * loc_random;
            double sty = j * distance * loc_random;
            double x_size = sizeu(e);
            double y_size = x_size;
            KBox l_box(stx, sty, x_size, y_size);
            buffer.emplace_back(l_box);
        }
        if (testrow > 500 && !(i % 100))std::cout << "50000 has generate..." << std::endl;
    }
}

void printvecvec(std::vector<std::vector<int>>& vec)
{
    std::cout << '[';
    for (auto& a : vec)
    {
        std::cout << '[';
        for (auto b = a.cbegin(); b != a.cend(); ++b)
        {
            std::cout << *b;
            if (b != --a.cend())std::cout << ", ";
        }
        std::cout << ']';
    }
    std::cout << "]\n";
}

}