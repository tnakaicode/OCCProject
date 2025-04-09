#define _WIN32_WINNT 0x0601 // Target Windows 7 or later
#include <windows.h>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <WNT_Window.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_PointCloud.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Writer.hxx>
#include <gp_Pnt.hxx>
#include <Quantity_NameOfColor.hxx>
#include <OSD_Environment.hxx>
#include <Graphic3d_AspectFillArea3d.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Quantity_Color.hxx>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include "Viewer.h"

int main()
{
    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 楕円の点群データを生成
    std::vector<std::array<float, 3>> ellipsePoints;
    const int numPoints = 1000000;
    const float radiusX = 2.0f; // 長軸
    const float radiusY = 1.0f; // 短軸
    const float radiusZ = 1.5f; // Z軸方向のスケール

    for (int i = 0; i < numPoints; ++i)
    {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;     // 0 to 2π
        float phi = acos(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f); // 0 to π
        float x = radiusX * sin(phi) * cos(theta);
        float y = radiusY * sin(phi) * sin(theta);
        float z = radiusZ * cos(phi);
        ellipsePoints.push_back({x, y, z});
    }

    // Viewerを初期化
    InitializeViewer("Point Cloud Viewer");

    // 点群を表示
    DisplayPointCloud(ellipsePoints);

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