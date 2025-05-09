#pragma once
#include <Poly_Triangulation.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfInteger.hxx>

class PolyProcessor
{
public:
    static Handle(Poly_Triangulation) CreateTriangulation(const TColgp_Array1OfPnt &vertices, const TColStd_Array1OfInteger &indices);
};