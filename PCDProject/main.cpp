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
#include <iostream>
#include <vector>
#include <cmath>
#include <array>

// グローバル変数
Handle(V3d_Viewer) viewer;
Handle(V3d_View) view;
Handle(AIS_InteractiveContext) context;

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool isDragging = false;  // ドラッグ中かどうか
    static bool isPanning = false;   // パン操作中かどうか
    static int lastX = 0, lastY = 0; // 前回のマウス位置

    switch (msg)
    {
    case WM_LBUTTONDOWN: // 左クリック押下
        isDragging = true;
        lastX = LOWORD(lParam);
        lastY = HIWORD(lParam);
        view->StartRotation(lastX, lastY); // 回転の開始
        SetCapture(hwnd);                  // マウスキャプチャを開始
        break;

    case WM_LBUTTONUP: // 左クリック解放
        isDragging = false;
        ReleaseCapture(); // マウスキャプチャを解除
        break;

    case WM_RBUTTONDOWN: // 右クリック押下
        isPanning = true;
        lastX = LOWORD(lParam);
        lastY = HIWORD(lParam);
        SetCapture(hwnd); // マウスキャプチャを開始
        break;

    case WM_RBUTTONUP: // 右クリック解放
        isPanning = false;
        ReleaseCapture(); // マウスキャプチャを解除
        break;

    case WM_MOUSEMOVE: // マウス移動
        if (isDragging && !view.IsNull())
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            view->Rotation(x, y); // 回転操作
            lastX = x;
            lastY = y;
        }
        else if (isPanning && !view.IsNull())
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            double dx = static_cast<double>(x - lastX);
            double dy = static_cast<double>(y - lastY);
            view->Pan(dx, dy); // パン操作
            lastX = x;
            lastY = y;
        }
        break;

    case WM_MOUSEWHEEL: // マウスホイール
        if (!view.IsNull())
        {
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta > 0)
            {
                view->SetZoom(0.9); // ズームイン
            }
            else
            {
                view->SetZoom(1.1); // ズームアウト
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// OpenCASCADEのViewerを初期化する関数
void InitializeViewer(const std::string &windowTitle)
{
    // Windows ウィンドウの初期化
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    const wchar_t *className = L"OpenCASCADEViewer"; // Unicode 文字列に変更

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
    {
        std::cerr << "Failed to register window class." << std::endl;
        return;
    }

    HWND hwnd = CreateWindowEx(
        0, className, L"OpenCASCADE Viewer", // Unicode 文字列に変更
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
    {
        std::cerr << "Failed to create window." << std::endl;
        return;
    }

    ShowWindow(hwnd, SW_SHOW);

    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    viewer = new V3d_Viewer(graphicDriver);
    viewer->SetDefaultLights();
    viewer->SetLightOn();

    view = viewer->CreateView();
    Handle(WNT_Window) wntWindow = new WNT_Window(hwnd);
    view->SetWindow(wntWindow);

    if (!wntWindow->IsMapped())
    {
        wntWindow->Map();
    }

    context = new AIS_InteractiveContext(viewer);
}

// 点群を表示する関数
void DisplayPointCloud(const std::vector<std::array<float, 3>> &points)
{
    // 点群データを TColgp_HArray1OfPnt に変換
    Handle(TColgp_HArray1OfPnt) pointArray = new TColgp_HArray1OfPnt(1, static_cast<Standard_Integer>(points.size()));
    for (Standard_Integer i = 0; i < points.size(); ++i)
    {
        const auto &point = points[i];
        pointArray->SetValue(i + 1, gp_Pnt(point[0], point[1], point[2]));
    }

    // AIS_PointCloud に点群を設定
    Handle(AIS_PointCloud) aisPointCloud = new AIS_PointCloud();
    aisPointCloud->SetPoints(pointArray);

    // 点群を表示
    context->Display(aisPointCloud, Standard_True);
    view->FitAll();
}

int main()
{
    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 球体の点群データを生成
    std::vector<std::array<float, 3>> spherePoints;
    const int numPoints = 10000;
    const float radius = 1.0f;

    for (int i = 0; i < numPoints; ++i)
    {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;     // 0 to 2π
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

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}