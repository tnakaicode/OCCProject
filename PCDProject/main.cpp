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

// グローバル変数
Handle(V3d_Viewer) viewer;
Handle(V3d_View) view;
Handle(AIS_InteractiveContext) context;

// OpenCASCADEのViewerを初期化する関数
void InitializeViewer(const std::string& windowTitle) {
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    viewer = new V3d_Viewer(graphicDriver);
    viewer->SetDefaultLights();
    viewer->SetLightOn();

    view = viewer->CreateView();
    Handle(WNT_Window) window = new WNT_Window(windowTitle.c_str());
    view->SetWindow(window);

    if (!window->IsMapped()) {
        window->Map();
    }

    context = new AIS_InteractiveContext(viewer);
}

// 点群を表示する関数
void DisplayPointCloud(const std::vector<std::array<float, 3>>& points) {
    Handle(AIS_PointCloud) aisPointCloud = new AIS_PointCloud();
    for (const auto& point : points) {
        gp_Pnt gpPoint(point[0], point[1], point[2]);
        aisPointCloud->SetPoints(1, &gpPoint);
    }

    context->Display(aisPointCloud, Standard_True);
    view->FitAll();
}

int main() {
    std::cout << "Starting Point Cloud Processing..." << std::endl;

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

    // Viewerを初期化
    InitializeViewer("Point Cloud Viewer");

    // 点群を表示
    DisplayPointCloud(spherePoints);

    std::cout << "Point Cloud Processing Completed." << std::endl;

    // イベントループを開始
    while (true) {
        // OpenCASCADEのイベント処理をここに追加
    }

    return 0;
}