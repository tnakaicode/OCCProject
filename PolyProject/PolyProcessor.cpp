#include "PolyProcessor.h"

Handle(Poly_Triangulation) PolyProcessor::CreateTriangulation(const TColgp_Array1OfPnt& vertices, const TColStd_Array1OfInteger& triangles) {
    return new Poly_Triangulation(vertices, triangles);
}