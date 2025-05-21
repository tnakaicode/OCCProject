#ifndef WIRE_ON_SURFACE_H
#define WIRE_ON_SURFACE_H

#include <vector>
#include <gp_Pnt2d.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Surface.hxx>

// Function to create a wire from a 2D polygon in UV space on a surface
TopoDS_Wire UVPolygon2DToWireOnSurface(const Handle(Geom_Surface) & surface,
                                       const std::vector<gp_Pnt2d> &uvPoints,
                                       bool close = true);

// Function to create a wire from a circle in UV space on a surface
TopoDS_Wire UVCircleToWireOnSurface(const Handle(Geom_Surface) & surface,
                                    const gp_Pnt2d &centerUV,
                                    double radius,
                                    int numSegments = 100);

// Function to create a wire from a spline in UV space on a surface
TopoDS_Wire UVSplineToWireOnSurface(const Handle(Geom_Surface) & surface,
                                    const std::vector<gp_Pnt2d> &uvPoints);

// Function to convert a wire on a surface to a wire in UV space
TopoDS_Wire SurfaceWireToUVWire(const TopoDS_Face &face, const TopoDS_Wire &wire);

// Function to check if points on an edge are on the given face
bool AreEdgePointsOnFace(const TopoDS_Edge &edge, const TopoDS_Face &face, int numPoints = 10);

#endif // WIRE_ON_SURFACE_H