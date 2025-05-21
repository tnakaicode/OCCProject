#include <iostream>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_Tool.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <Standard_Real.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <gp_Cylinder.hxx>
#include <TopoDS.hxx>

#include "WireOnSurface.h"
#include "Viewer.h"

int main()
{
    // Viewerの初期化
    Handle(V3d_Viewer) viewer;
    Handle(V3d_View) view;
    Handle(AIS_InteractiveContext) context;

    // 円筒面の作成
    Handle(Geom_CylindricalSurface) cylinder = new Geom_CylindricalSurface(gp_Ax3(), 5.0);
    TopoDS_Face face = BRepBuilderAPI_MakeFace(cylinder->Cylinder(), 0, 2 * M_PI, -10, 10).Face();

    // UV空間上の四角形の定義
    std::vector<gp_Pnt2d> uvPoly = {
        gp_Pnt2d(0, 1),
        gp_Pnt2d(M_PI, 1),
        gp_Pnt2d(M_PI, 5),
        gp_Pnt2d(M_PI + M_PI / 4, 7),
        gp_Pnt2d(M_PI + M_PI / 4, 1),
        gp_Pnt2d(2 * M_PI, 1)};

    // UV空間上の多角形をサーフェス上のワイヤーに変換
    TopoDS_Wire wire = UVPolygon2DToWireOnSurface(cylinder, uvPoly, false);

    // ワイヤー内のエッジを確認
    for (TopExp_Explorer explorer(wire, TopAbs_EDGE); explorer.More(); explorer.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
        std::cout << "Checking edge..." << std::endl;
        bool result = AreEdgePointsOnFace(edge, face);
        if (result)
        {
            std::cout << "All points on this edge are on the face." << std::endl;
        }
        else
        {
            std::cout << "Some points on this edge are NOT on the face." << std::endl;
        }
    }

    // サーフェス上のワイヤーをUV平面上のワイヤーに変換
    TopoDS_Wire uvWire = SurfaceWireToUVWire(face, wire);

    // 結果を出力
    if (!uvWire.IsNull())
    {
        std::cout << "UV Wire successfully created." << std::endl;
    }
    else
    {
        std::cout << "Failed to create UV Wire." << std::endl;
    }

    return 0;
}