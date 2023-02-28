#pragma once
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_OffsetCurve.hxx>

#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>

#include <GeomTools.hxx>
#include <GeomTools_CurveSet.hxx>

int test()
{
    gp_Ax2 axis(gp_Pnt(1, 2, 3), gp::DZ());
    std::ofstream dumpFile("geometryCurve.txt");

    // 3D curve record 1: Line.
    // Example: 1 1 0 3 0 1 0 
    Handle_Geom_Line theLine = new Geom_Line(gp_Pnt(1, 0, 3), gp_Dir(0, 1, 0));
    GeomTools::Write(theLine, dumpFile);
    GeomTools::Dump(theLine, dumpFile);
    GeomTools::Dump(theLine, std::cout);

    // 3D curve record 2: Circle.
    // Example: 2 1 2 3 0 0 1 1 0 -0 -0 1 0 4
    Handle_Geom_Circle theCircle = new Geom_Circle(axis, 4.0);
    GeomTools::Write(theCircle, dumpFile);
    GeomTools::Dump(theCircle, dumpFile);
    GeomTools::Dump(theCircle, std::cout);

    // 3D curve record 3: Ellipse.
    // Example: 3 1 2 3 0 0 1 1 0 -0 -0 1 0 5 4
    Handle_Geom_Ellipse theEllipse = new Geom_Ellipse(axis, 5.0, 4.0);
    GeomTools::Write(theEllipse, dumpFile);
    GeomTools::Dump(theEllipse, dumpFile);
    GeomTools::Dump(theEllipse, std::cout);

    // 3D curve record 4: Parabola.
    // Example: 4 1 2 3 0 0 1 1 0 -0 -0 1 0 16
    Handle_Geom_Parabola theParabola = new Geom_Parabola(axis, 16.0);
    GeomTools::Write(theParabola, dumpFile);
    GeomTools::Dump(theParabola, dumpFile);
    GeomTools::Dump(theParabola, std::cout);

    // 3D curve record 5: Hyperbola.
    // Example: 5 1 2 3 0 0 1 1 0 -0 -0 1 0 5 4
    Handle_Geom_Hyperbola theHyperbola = new Geom_Hyperbola(axis, 5.0, 4.0);
    GeomTools::Write(theHyperbola, dumpFile);
    GeomTools::Dump(theHyperbola, dumpFile);
    GeomTools::Dump(theHyperbola, std::cout);

    // 3D curve record 6: Bezier Curve.
    // Example: 6 1 2 0 1 0  4 1 -2 0  5 2 3 0  6 
    TColgp_Array1OfPnt poles(1, 3);
    TColStd_Array1OfReal weights(1, 3);

    poles.SetValue(1, gp_Pnt(0, 1, 0));
    poles.SetValue(2, gp_Pnt(1, -2, 0));
    poles.SetValue(3, gp_Pnt(2, 3, 0));

    weights.SetValue(1, 4.0);
    weights.SetValue(2, 5.0);
    weights.SetValue(3, 6.0);

    Handle_Geom_BezierCurve theBezierCurve = new Geom_BezierCurve(poles, weights);
    GeomTools::Write(theBezierCurve, dumpFile);
    GeomTools::Dump(theBezierCurve, dumpFile);
    GeomTools::Dump(theBezierCurve, std::cout);

    // 3D curve record 7: B-Spline Curve.
    // Example: 7 1 0  1 3 5  0 1 0  4 1 -2 0  5 2 3 0  6
    //          0 1 0.25 1 0.5 1 0.75 1 1 1
    Standard_Integer degree = 1;
    TColStd_Array1OfReal knots(1, 5);
    TColStd_Array1OfInteger multiplicities(1, 5);

    knots.SetValue(1, 0);
    knots.SetValue(2, 0.25);
    knots.SetValue(3, 0.5);
    knots.SetValue(4, 0.75);
    knots.SetValue(5, 1.0);

    // all knots multiplicity of the B-spline is 1.
    multiplicities.Init(1);

    Handle_Geom_BSplineCurve theBSplineCurve = new Geom_BSplineCurve(poles, weights, knots, multiplicities, degree);
    GeomTools::Write(theBSplineCurve, dumpFile);
    GeomTools::Dump(theBSplineCurve, dumpFile);
    GeomTools::Dump(theBSplineCurve, std::cout);

    // 3D curve record 8: Trimmed Curve.
    // Example: 8 -4 5
    //          1 1 2 3 1 0 0 
    Handle_Geom_Line theBaseCurve = new Geom_Line(gp_Pnt(1, 2, 3), gp_Dir(1, 0, 0));
    Handle_Geom_TrimmedCurve theTrimmedCurve = new Geom_TrimmedCurve(theBaseCurve, -4, 5);
    GeomTools::Write(theTrimmedCurve, dumpFile);
    GeomTools::Dump(theTrimmedCurve, dumpFile);
    GeomTools::Dump(theTrimmedCurve, std::cout);

    // 3D curve record 9: Offset Curve.
    // Example: 9 2
    //          0 1 0 
    //          1 1 2 3 1 0 0 
    Handle_Geom_OffsetCurve theOffsetCurve = new Geom_OffsetCurve(theBaseCurve, 2.0, gp::DY());
    GeomTools::Write(theOffsetCurve, dumpFile);
    GeomTools::Dump(theOffsetCurve, dumpFile);
    GeomTools::Dump(theOffsetCurve, std::cout);

    return 0;
}


#include <gp_Dir2d.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Pnt2d.hxx>
#include <GCE2d_MakeLine.hxx>
#include <IntAna2d_AnaIntersection.hxx>

void testcross(void)
{
    GCE2d_MakeLine aLineMaker1(gp_Pnt2d(0.0, 0.0), gp_Pnt2d(10.0, 10.0));
    GCE2d_MakeLine aLineMaker2(gp_Pnt2d(2.0, 10.0), gp_Pnt2d(12.0, 2.0));

    gp_Lin2d aLine1 = aLineMaker1.Value()->Lin2d();
    gp_Lin2d aLine2 = aLineMaker2.Value()->Lin2d();

    IntAna2d_AnaIntersection aIntAna;
    aIntAna.Perform(aLine1, aLine2);
    if (aIntAna.IsDone())
    {
        const IntAna2d_IntPoint& aIntPoint = aIntAna.Point(1);
        std::cout << "Number of IntPoint between the 2 curves: "
            << aIntAna.NbPoints() << std::endl;
        std::cout << "Intersect Point: " << aIntPoint.Value().X()
            << ", " << aIntPoint.Value().Y() << std::endl;
    }
}
