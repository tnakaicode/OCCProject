#include <iostream>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include "PolyProcessor.h"

int main()
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

    // テーパー付き円柱（円錐台）のパラメータ
    double radiusBottom = 1.0; // 下部円の半径
    double radiusTop = 0.5;    // 上部円の半径
    double height = 2.0;       // 高さ

    // テーパー付き円柱（円錐台）を作成
    BRepPrimAPI_MakeCone coneMaker(radiusBottom, radiusTop, height);
    TopoDS_Shape coneShape = coneMaker.Shape();

    // 三角形分割を取得
    TopExp_Explorer faceExplorer(coneShape, TopAbs_FACE);
    while (faceExplorer.More())
    {
        TopoDS_Face face = TopoDS::Face(static_cast<const TopoDS_Shape &>(faceExplorer.Current()));
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