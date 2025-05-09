#include <iostream>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Pnt.hxx>
#include <gp_Circ.hxx>
#include <GC_MakeCircle.hxx>
#include "PolyProcessor.h"

int test()
{
    // Define the vertices of a polygon (e.g., a square)
    TColgp_Array1OfPnt vertices(1, 4);
    vertices(1) = gp_Pnt(0.0, 0.0, 0.0);
    vertices(2) = gp_Pnt(1.0, 0.0, 0.0);
    vertices(3) = gp_Pnt(1.0, 1.0, 0.0);
    vertices(4) = gp_Pnt(0.0, 1.0, 0.0);

    // Define the triangle indices
    TColStd_Array1OfInteger indices(1, 6); // 2 triangles * 3 indices
    indices(1) = 1;
    indices(2) = 2;
    indices(3) = 3; // Triangle 1
    indices(4) = 1;
    indices(5) = 3;
    indices(6) = 4; // Triangle 2

    // Create the Poly_Triangulation object
    Handle(Poly_Triangulation) triangulation = PolyProcessor::CreateTriangulation(vertices, indices);

    // Output the triangulation data
    std::cout << "Number of Nodes: " << triangulation->NbNodes() << std::endl;
    std::cout << "Number of Triangles: " << triangulation->NbTriangles() << std::endl;

    // Print the vertices
    std::cout << "Vertices:" << std::endl;
    for (int i = 1; i <= triangulation->NbNodes(); ++i)
    {
        gp_Pnt p = triangulation->Node(i);
        std::cout << "  Vertex " << i << ": (" << p.X() << ", " << p.Y() << ", " << p.Z() << ")" << std::endl;
    }

    // Print the triangles
    std::cout << "Triangles:" << std::endl;
    for (int i = 1; i <= triangulation->NbTriangles(); ++i)
    {
        Standard_Integer n1, n2, n3;
        triangulation->Triangle(i).Get(n1, n2, n3);
        std::cout << "  Triangle " << i << ": (" << n1 << ", " << n2 << ", " << n3 << ")" << std::endl;
    }
    return 0;
}

int main()
{
    // テーパー付き円柱（円錐台）のパラメータ
    double radiusBottom = 1.0; // 下部円の半径
    double radiusTop = 0.5;    // 上部円の半径
    double height = 2.0;       // 高さ

    // 下部円を作成
    gp_Ax2 bottomAx2(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0.0, 0.0, 1.0)); // 底面の座標系
    gp_Circ bottomCircle(bottomAx2, radiusBottom);

    // 上部円を作成
    gp_Ax2 topAx2(gp_Pnt(0.0, 0.0, height), gp_Dir(0.0, 0.0, 1.0)); // 上面の座標系
    gp_Circ topCircle(topAx2, radiusTop);

    // 下部円の輪郭を作成
    Handle(Geom_Circle) geomBottomCircle = new Geom_Circle(bottomCircle);
    TopoDS_Wire bottomWire = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(geomBottomCircle));

    // 上部円の輪郭を作成
    Handle(Geom_Circle) geomTopCircle = new Geom_Circle(topCircle);
    TopoDS_Wire topWire = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(geomTopCircle));

    // ThruSectionsで円柱を作成
    BRepOffsetAPI_ThruSections thruSections(true, true, 1.0e-6); // Solid, ruled, precision
    thruSections.AddWire(bottomWire);
    thruSections.AddWire(topWire);
    thruSections.Build();

    // 成功したかどうかを確認
    if (!thruSections.IsDone())
    {
        std::cerr << "Error: ThruSections failed to build the shape." << std::endl;
        return 1; // エラー終了
    }

    TopoDS_Shape coneShape = thruSections.Shape();

    // 三角形分割を取得
    TopExp_Explorer faceExplorer(coneShape, TopAbs_FACE);
    while (faceExplorer.More())
    {
        TopoDS_Face face = TopoDS::Face(faceExplorer.Current());
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, TopLoc_Location());

        if (!triangulation.IsNull())
        {
            // 頂点数と三角形数を出力
            std::cout << "Number of Nodes: " << triangulation->NbNodes() << std::endl;
            std::cout << "Number of Triangles: " << triangulation->NbTriangles() << std::endl;

            // 頂点を出力
            std::cout << "Vertices:" << std::endl;
            for (int i = 1; i <= triangulation->NbNodes(); ++i)
            {
                gp_Pnt p = triangulation->Node(i);
                std::cout << "  Vertex " << i << ": (" << p.X() << ", " << p.Y() << ", " << p.Z() << ")" << std::endl;
            }

            // 三角形を出力
            std::cout << "Triangles:" << std::endl;
            for (int i = 1; i <= triangulation->NbTriangles(); ++i)
            {
                Standard_Integer n1, n2, n3;
                triangulation->Triangle(i).Get(n1, n2, n3);
                std::cout << "  Triangle " << i << ": (" << n1 << ", " << n2 << ", " << n3 << ")" << std::endl;
            }
        }

        faceExplorer.Next();
    }

    return 0;
}