#define _WIN32_WINNT 0x0601 // Target Windows 7 or later
#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include "Viewer.h"
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Quaternion.hxx>

int main()
{
    // Viewerを初期化
    Handle(V3d_Viewer) viewer;
    Handle(V3d_View) view;
    Handle(AIS_InteractiveContext) context;
    InitializeViewer("Point Cloud Viewer", viewer, view, context);
    DisplayViewCube(context, view);

    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 新しい座標系を定義（例: 平行移動 + 回転）
    gp_Pnt origin(1.0, 2.0, 0.5); // 新しい座標系の原点
    gp_Dir zDir(0.0, 0.0, 1.0);   // Z軸方向
    gp_Dir xDir(1.0, 0.0, 0.0);   // X軸方向
    gp_Ax3 newAxis(origin, zDir, xDir);

    // 座標変換行列を取得
    gp_Trsf transform;
    transform.SetTransformation(newAxis);

    // 楕円の点群データを生成
    std::vector<gp_Pnt> ellipsePoints;
    const int numPoints = 10'000'000; // 点の数
    const float radiusX = 2.0f;       // 長軸
    const float radiusY = 1.0f;       // 短軸
    const float radiusZ = 1.5f;       // Z軸方向のスケール

    for (int i = 0; i < numPoints; ++i)
    {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;     // 0 to 2π
        float phi = acos(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f); // 0 to π
        float x = radiusX * sin(phi) * cos(theta);
        float y = radiusY * sin(phi) * sin(theta);
        float z = radiusZ * cos(phi);
        gp_Pnt point(x, y, z);
        point.Transform(transform);     // 座標変換を適用
        ellipsePoints.push_back(point); // 座標変換を適用
    }

    // 楕円体を表示
    DisplayEllipsoid(context, radiusX, radiusY, radiusZ);

    // 点群を表示
    DisplayPointCloud(context, ellipsePoints);

    // 楕円体を覆う長方形の頂点を計算
    std::vector<gp_Pnt> boundingBoxVertices = {
        gp_Pnt(-radiusX, -radiusY, -radiusZ), // 頂点1
        gp_Pnt(radiusX, -radiusY, -radiusZ),  // 頂点2
        gp_Pnt(radiusX, radiusY, -radiusZ),   // 頂点3
        gp_Pnt(-radiusX, radiusY, -radiusZ),  // 頂点4
        gp_Pnt(-radiusX, -radiusY, radiusZ),  // 頂点5
        gp_Pnt(radiusX, -radiusY, radiusZ),   // 頂点6
        gp_Pnt(radiusX, radiusY, radiusZ),    // 頂点7
        gp_Pnt(-radiusX, radiusY, radiusZ)    // 頂点8
    };

    // 変換を適用した頂点を描画
    for (auto &vertex : boundingBoxVertices)
    {
        vertex.Transform(transform); // 座標変換を適用
        DisplayPoint(context, vertex);
    }
    view->FitAll(); // 全体を表示

    std::cout << "Point Cloud Processing Completed." << std::endl;

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}