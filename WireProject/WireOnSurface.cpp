#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom2dAPI_Interpolate.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <GCE2d_MakeCircle.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir2d.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <TColgp_HArray1OfPnt2d.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Surface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <vector>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>

// Function to create a wire from a 2D polygon in UV space on a surface
TopoDS_Wire UVPolygon2DToWireOnSurface(const Handle(Geom_Surface) & surface,
                                       const std::vector<gp_Pnt2d> &uvPoints,
                                       bool close = true)
{
    BRepBuilderAPI_MakeWire wireMaker;
    size_t n = uvPoints.size();

    for (size_t i = 0; i < (close ? n : n - 1); ++i)
    {
        gp_Pnt2d p1 = uvPoints[i];
        gp_Pnt2d p2 = uvPoints[(i + 1) % n];
        Handle(Geom2d_Curve) seg2d = GCE2d_MakeSegment(p1, p2).Value();
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(seg2d, surface, seg2d->FirstParameter(), seg2d->LastParameter()).Edge();
        wireMaker.Add(edge);
    }

    return wireMaker.Wire();
}

// Function to create a wire from a circle in UV space on a surface
TopoDS_Wire UVCircleToWireOnSurface(const Handle(Geom_Surface) & surface,
                                    const gp_Pnt2d &centerUV,
                                    double radius,
                                    int numSegments = 100)
{
    gp_Ax2d axis(centerUV, gp_Dir2d(1, 0));
    gp_Circ2d circle(axis, radius);
    Handle(Geom2d_Curve) circleEdge = GCE2d_MakeCircle(circle).Value();

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(circleEdge, surface).Edge();

    BRepBuilderAPI_MakeWire wireMaker;
    wireMaker.Add(edge);

    return wireMaker.Wire();
}

// Function to create a wire from a spline in UV space on a surface
TopoDS_Wire UVSplineToWireOnSurface(const Handle(Geom_Surface) & surface,
                                    const std::vector<gp_Pnt2d> &uvPoints)
{
    TColgp_Array1OfPnt2d array(1, static_cast<Standard_Integer>(uvPoints.size()));
    for (size_t i = 0; i < uvPoints.size(); ++i)
    {
        array.SetValue(static_cast<Standard_Integer>(i + 1), uvPoints[i]);
    }

    Handle(TColgp_HArray1OfPnt2d) hArray = new TColgp_HArray1OfPnt2d(array);
    Geom2dAPI_Interpolate curveBuilder(hArray, true, 1.0e-6);
    curveBuilder.Perform();
    Handle(Geom2d_Curve) spline2d = Handle(Geom2d_Curve)::DownCast(curveBuilder.Curve());

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(spline2d, surface).Edge();

    BRepBuilderAPI_MakeWire wireMaker;
    wireMaker.Add(edge);

    return wireMaker.Wire();
}

// Function to convert a wire on a surface to a wire in UV space
TopoDS_Wire SurfaceWireToUVWire(const TopoDS_Face &face, const TopoDS_Wire &wire)
{
    BRepBuilderAPI_MakeWire wireMaker;

    for (TopExp_Explorer explorer(wire, TopAbs_EDGE); explorer.More(); explorer.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(explorer.Current());

        Standard_Real first, last;
        Standard_Boolean isStored;

        Handle(Geom2d_Curve) curve2d = BRep_Tool::CurveOnSurface(edge, face, first, last, &isStored);

        if (!curve2d.IsNull())
        {
            Handle(Geom2d_TrimmedCurve) trimmedCurve = new Geom2d_TrimmedCurve(curve2d, first, last);
            // TopoDS_Edge uvEdge = BRepBuilderAPI_MakeEdge(trimmedCurve, face).Edge();
            // wireMaker.Add(uvEdge);
        }
        wireMaker.Add(edge);
    }

    return wireMaker.Wire();
}