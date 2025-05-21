#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Tool.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>

TopoDS_Wire SurfaceWireToUVWire(const TopoDS_Face& face, const TopoDS_Wire& wire) {
    BRepBuilderAPI_MakeWire wireMaker;

    for (TopExp_Explorer explorer(wire, TopAbs_EDGE); explorer.More(); explorer.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(explorer.Current());

        Standard_Real first, last;
        Standard_Boolean isStored;

        Handle(Geom2d_Curve) curve2d = BRep_Tool::CurveOnSurface(edge, face, first, last, &isStored);

        if (!curve2d.IsNull()) {
            Handle(Geom2d_TrimmedCurve) trimmedCurve = new Geom2d_TrimmedCurve(curve2d, first, last);
            TopoDS_Edge uvEdge = BRepBuilderAPI_MakeEdge(trimmedCurve).Edge();
            wireMaker.Add(uvEdge);
        }
    }

    return wireMaker.Wire();
}