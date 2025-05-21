#define _WIN32_WINNT 0x0601 // Target Windows 7 or later
#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>

// Include necessary Open CASCADE headers
#include <BRep_Tool.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Pnt.hxx>
#include <gp_Circ.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include "PolyProcessor.h"
#include "Viewer.h"

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

TopoDS_Wire CreateCicleWire(const gp_Ax2 &topAx2, double radiusTop, double alpha1, double alpha2)
{
    // 半円または部分円を作成
    Handle(Geom_TrimmedCurve) geomTopArc = GC_MakeArcOfCircle(
        gp_Circ(topAx2, radiusTop), alpha1, alpha2, true);

    // 部分円の端点を取得
    gp_Pnt startPoint = geomTopArc->StartPoint();
    gp_Pnt endPoint = geomTopArc->EndPoint();

    // `topAx2`の原点を取得
    gp_Pnt toporigin = topAx2.Location();

    // 部分円のエッジを作成
    TopoDS_Edge topArcEdge = BRepBuilderAPI_MakeEdge(geomTopArc);

    // ワイヤーを作成
    if (std::abs(alpha2 - alpha1) < 2 * M_PI)
    {
        // 部分円の場合、原点を経由して端点を結ぶエッジを作成
        TopoDS_Edge edgeToStart = BRepBuilderAPI_MakeEdge(toporigin, startPoint);
        TopoDS_Edge edgeToEnd = BRepBuilderAPI_MakeEdge(toporigin, endPoint);

        // 部分円と原点を経由するエッジをワイヤーでつなぐ
        return BRepBuilderAPI_MakeWire(topArcEdge, edgeToStart, edgeToEnd);
    }
    else
    {
        // 完全な円の場合、エッジは不要
        return BRepBuilderAPI_MakeWire(topArcEdge);
    }
}

int main()
{
    // Viewerの初期化
    Handle(V3d_Viewer) viewer;
    Handle(V3d_View) view;
    Handle(AIS_InteractiveContext) context;
    InitializeViewer("Poly Triangulation Viewer", viewer, view, context);

    // テーパー付き円柱（円錐台）のパラメータ
    double radiusBottom = 10.0; // 下部円の半径
    double radiusTop = 5.0;     // 上部円の半径
    double height = 8.0;        // 高さ

    // 下部円を作成
    gp_Ax2 bottomAx2(gp_Pnt(0.0, 0.0, 0.0),
                     gp_Dir(0.0, 0.0, 1.0),
                     gp_Dir(1.0, 0.0, 0.0));
    TopoDS_Wire bottomWire = CreateCicleWire(bottomAx2, radiusBottom, 0, M_PI * 2);

    // 上部円を作成（半円に変更）
    gp_Ax2 topAx2(gp_Pnt(1.0, 2.0, height),
                  gp_Dir(0.0, 0.1, 1.0),
                  gp_Dir(0.0, 1.0, 0.0));
    // 上部部分円のワイヤーを作成（例: 120度の部分円）
    double alpha1 = M_PI * (-1 / 10);
    double alpha2 = M_PI * (1 + 1 / 3.0);
    TopoDS_Wire topWire = CreateCicleWire(topAx2, radiusTop, alpha1, alpha2);

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

    // 体積と面積を計算
    GProp_GProps props;
    BRepGProp::VolumeProperties(coneShape, props); // 体積計算
    double volume = props.Mass();                  // 体積は "Mass" として取得される

    BRepGProp::SurfaceProperties(coneShape, props); // 面積計算
    double surfaceArea = props.Mass();              // 面積も "Mass" として取得される

    // 結果を出力
    std::cout << "Volume: " << volume << std::endl;
    std::cout << "Surface Area: " << surfaceArea << std::endl;

    // Poly_Triangulationを格納するリスト
    std::vector<Handle(Poly_Triangulation)> triangulations;

    // メッシュ化
    BRepMesh_IncrementalMesh mesher(coneShape, 0.1); // 0.1はメッシュの精度
    mesher.Perform();

    // 三角形分割を取得
    TopExp_Explorer faceExplorer(coneShape, TopAbs_FACE);
    int faceIndex = 1; // 面のインデックス
    while (faceExplorer.More())
    {
        TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

        // 面積を計算
        GProp_GProps faceProps;
        BRepGProp::SurfaceProperties(face, faceProps);
        double faceArea = faceProps.Mass(); // 面積は "Mass" として取得される

        // 面の情報を出力
        std::cout << "Face " << faceIndex << " Area: " << faceArea << std::endl;

        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, TopLoc_Location());
        if (!triangulation.IsNull())
        {
            triangulations.push_back(triangulation);
        }
        faceExplorer.Next();
    }

    // Viewerで描画
    for (const auto &triangulation : triangulations)
    {
        DisplayPolyTriangulation(context, triangulation);
    }

    // Viewerを表示
    view->FitAll();

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}