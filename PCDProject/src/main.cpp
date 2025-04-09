#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <AIS_PointCloud.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_Handle.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <gp_Pnt.hxx>
#include "PointCloudProcessor.h"

int main() {
    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 点群処理クラスのインスタンスを作成
    PointCloudProcessor processor;

    // 球体の点群データを生成
    std::vector<std::array<float, 3>> spherePoints;
    const int numPoints = 10000;
    const float radius = 1.0f;

    for (int i = 0; i < numPoints; ++i) {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI; // 0 to 2π
        float phi = acos(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f); // 0 to π
        float x = radius * sin(phi) * cos(theta);
        float y = radius * sin(phi) * sin(theta);
        float z = radius * cos(phi);
        spherePoints.push_back({x, y, z});
    }

    // OpenCASCADEのViewerを初期化
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);
    Handle(V3d_Viewer) viewer = new V3d_Viewer(graphicDriver);
    Handle(V3d_View) view = viewer->CreateView();
    Handle(WNT_Window) window = new WNT_Window("Point Cloud Viewer", nullptr);
    view->SetWindow(window);

    if (!window->IsMapped()) {
        window->Map();
    }

    // 点群をAIS_PointCloudに変換
    Handle(AIS_PointCloud) aisPointCloud = new AIS_PointCloud();
    for (const auto& point : spherePoints) {
        gp_Pnt gpPoint(point[0], point[1], point[2]);
        aisPointCloud->AddPoints(1, &gpPoint);
    }

    // 点群を表示
    Handle(AIS_InteractiveContext) context = new AIS_InteractiveContext(viewer);
    context->Display(aisPointCloud, Standard_True);
    view->FitAll();

    std::cout << "Point Cloud Processing Completed." << std::endl;

    // イベントループを開始
    while (true) {
        // OpenCASCADEのイベント処理をここに追加
    }

    return 0;
}