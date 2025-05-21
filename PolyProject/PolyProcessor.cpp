#include "PolyProcessor.h"
#include <Poly_Triangle.hxx>

Handle(Poly_Triangulation) PolyProcessor::CreateTriangulation(const TColgp_Array1OfPnt &vertices, const TColStd_Array1OfInteger &indices)
{
    // Convert TColStd_Array1OfInteger to Poly_Array1OfTriangle
    Standard_Integer numTriangles = indices.Length() / 3;
    Poly_Array1OfTriangle triangles(1, numTriangles);

    for (Standard_Integer i = 1; i <= numTriangles; ++i)
    {
        Standard_Integer n1 = indices((i - 1) * 3 + 1);
        Standard_Integer n2 = indices((i - 1) * 3 + 2);
        Standard_Integer n3 = indices((i - 1) * 3 + 3);
        triangles.SetValue(i, Poly_Triangle(n1, n2, n3));
    }

    // Create and return the Poly_Triangulation object
    return new Poly_Triangulation(vertices, triangles);
}