#include "Viewer.h"
#include <AIS_ViewCube.hxx>
#include <AIS_AnimationCamera.hxx>
#include <Graphic3d_TransformPers.hxx>
#include <Graphic3d_ZLayerId.hxx>
#include <TCollection_AsciiString.hxx>
#include <iostream>

// グローバル変数
Handle(V3d_Viewer) viewer;
Handle(V3d_View) view;
Handle(AIS_InteractiveContext) context;

// 座標系を表示する関数
void DisplayViewCube()
{
    Handle(AIS_ViewCube) viewCube = new AIS_ViewCube();
    viewCube->SetSize(50.0);                         // サイズを設定
    viewCube->SetBoxColor(Quantity_NOC_GRAY);        // ボックスの色を設定
    viewCube->SetDrawAxes(Standard_True);            // 軸を描画
    viewCube->SetTransparency(0.5);                  // 透明度を設定
    viewCube->SetZLayer(Graphic3d_ZLayerId_Topmost); // 最上位レイヤーに設定

    // オフセットを調整して右下に配置
    Graphic3d_Vec2i offset(100, 100); // ウィンドウの右下からのオフセット（ピクセル単位）
    viewCube->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers, Aspect_TOTP_RIGHT_LOWER, offset));

    // アニメーションを有効化
    Handle(AIS_AnimationCamera) animation = new AIS_AnimationCamera("ViewCubeAnimation", view);
    viewCube->SetViewAnimation(animation);

    // AIS_ViewCube をコンテキストに追加
    context->Display(viewCube, Standard_True);
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool isDragging = false;
    static bool isPanning = false;
    static int lastX = 0, lastY = 0;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        // マウスクリックで選択を処理
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(hwnd, &point);

        // MoveToでオブジェクトを検出
        context->MoveTo(point.x, point.y, view, Standard_True);

        // 検出されたオブジェクトを選択
        if (context->HasDetected())
        {
            context->SelectDetected();
            Handle(AIS_InteractiveObject) detectedObject = context->DetectedInteractive();
            if (!detectedObject.IsNull() && detectedObject->IsKind(STANDARD_TYPE(AIS_ViewCube)))
            {
                // ViewCube のクリックイベントを処理
                std::cout << "ViewCube clicked!" << std::endl;
            }
        }
        else
        {
            isDragging = true;
            lastX = LOWORD(lParam);
            lastY = HIWORD(lParam);
            view->StartRotation(lastX, lastY);
        }
    }
        SetCapture(hwnd);
        break;

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;

    case WM_RBUTTONDOWN:
        isPanning = true;
        lastX = LOWORD(lParam);
        lastY = HIWORD(lParam);
        SetCapture(hwnd);
        break;

    case WM_RBUTTONUP:
        isPanning = false;
        ReleaseCapture();
        break;

    case WM_MOUSEMOVE:
        if (isDragging && !view.IsNull())
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            view->Rotation(x, y);
            lastX = x;
            lastY = y;
        }
        else if (isPanning && !view.IsNull())
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            double dx = static_cast<double>(x - lastX);
            double dy = static_cast<double>(y - lastY);
            view->Pan(dx, dy);
            lastX = x;
            lastY = y;
        }
        break;

    case WM_MOUSEWHEEL:
        if (!view.IsNull())
        {
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta < 0)
            {
                view->SetZoom(0.9);
            }
            else
            {
                view->SetZoom(1.1);
            }
        }
        break;

    case WM_SIZE:
        if (!view.IsNull())
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            if (width > 0 && height > 0)
            {
                view->SetWindow(view->Window());
                view->MustBeResized();
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

// Viewerを初期化する関数
void InitializeViewer(const std::string &windowTitle)
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    const wchar_t *className = L"OpenCASCADEViewer";

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
        0, className, std::wstring(windowTitle.begin(), windowTitle.end()).c_str(),
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

    // 座標系を表示
    DisplayViewCube();
}

// 点群を表示する関数
void DisplayPointCloud(const std::vector<std::array<float, 3>> &points)
{
    Handle(TColgp_HArray1OfPnt) pointArray = new TColgp_HArray1OfPnt(1, static_cast<Standard_Integer>(points.size()));
    for (Standard_Integer i = 0; i < points.size(); ++i)
    {
        const auto &point = points[i];
        pointArray->SetValue(i + 1, gp_Pnt(point[0], point[1], point[2]));
    }

    Handle(AIS_PointCloud) aisPointCloud = new AIS_PointCloud();
    aisPointCloud->SetPoints(pointArray);

    context->Display(aisPointCloud, Standard_True);
    view->FitAll();
}
