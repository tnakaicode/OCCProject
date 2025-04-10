#define _WIN32_WINNT 0x0601 // Target Windows 7 or later
#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include "Viewer.h"

int main()
{
    // Viewerを初期化
    Handle(V3d_Viewer) viewer;
    Handle(V3d_View) view;
    Handle(AIS_InteractiveContext) context;
    InitializeViewer("Point Cloud Viewer", viewer, view, context);
    DisplayViewCube(context, view);

    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 楕円の点群データを生成
    std::vector<std::array<float, 3>> ellipsePoints;
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
        ellipsePoints.push_back({x, y, z});
    }

    // 楕円体を表示
    DisplayEllipsoid(context, radiusX, radiusY, radiusZ);

    // 点群を表示
    DisplayPointCloud(context, ellipsePoints);

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