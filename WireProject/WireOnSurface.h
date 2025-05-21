#ifndef SURFACE_WIRE_TO_UV_WIRE_H
#define SURFACE_WIRE_TO_UV_WIRE_H

#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>

TopoDS_Wire SurfaceWireToUVWire(const TopoDS_Face& face, const TopoDS_Wire& wire);

#endif // SURFACE_WIRE_TO_UV_WIRE_H