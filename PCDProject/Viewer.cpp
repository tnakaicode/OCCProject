#include "Viewer.h"
#include <AIS_ViewCube.hxx>
#include <AIS_AnimationCamera.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_PointCloud.hxx>
#include <Graphic3d_AspectFillArea3d.hxx>
#include <Graphic3d_TransformPers.hxx>
#include <Graphic3d_ZLayerId.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <WNT_Window.hxx>
#include <TCollection_AsciiString.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Elips.hxx>
#include <BRepTools.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <STEPControl_Writer.hxx>
#include <Quantity_NameOfColor.hxx>
#include <OSD_Environment.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Quantity_Color.hxx>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

// 設定ファイルのパス
const std::string CONFIG_FILE = "viewer_config.txt";

// ウィンドウのサイズと位置を保存
void SaveWindowConfig(HWND hwnd)
{
    RECT rect;
    if (GetWindowRect(hwnd, &rect))
    {
        std::ofstream configFile(CONFIG_FILE, std::ios::out);
        if (configFile.is_open())
        {
            configFile << rect.left << " " << rect.top << " "
                       << (rect.right - rect.left) << " " << (rect.bottom - rect.top) << std::endl;
            configFile.close();
        }
    }
}

// ウィンドウのサイズと位置を読み込み
void LoadWindowConfig(HWND hwnd)
{
    std::ifstream configFile(CONFIG_FILE, std::ios::in);
    if (configFile.is_open())
    {
        int x, y, width, height;
        if (configFile >> x >> y >> width >> height)
        {
            SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        configFile.close();
    }
}

// 構造体で `context` と `view` をまとめて管理
struct ViewerData
{
    Handle(AIS_InteractiveContext) context;
    Handle(V3d_View) view;
};

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ViewerData *viewerData = reinterpret_cast<ViewerData *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg)
    {
    case WM_CREATE:
        // ウィンドウ作成時に設定ファイルからサイズと位置を読み込む
        LoadWindowConfig(hwnd);
        break;

    case WM_LBUTTONDOWN:
    {
        if (viewerData)
        {
            Handle(AIS_InteractiveContext) context = viewerData->context;
            Handle(V3d_View) view = viewerData->view;

            // 左ボタン押下時の処理
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
        }
    }
        SetCapture(hwnd); // マウスキャプチャを設定
        break;

    case WM_DESTROY:
        // ウィンドウが破棄されるときの処理
        SaveWindowConfig(hwnd); // ウィンドウのサイズと位置を保存

        // `ViewerData` を解放
        delete viewerData;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

        PostQuitMessage(0); // アプリケーションを終了
        break;

    default:
        // その他のメッセージはデフォルトの処理を実行
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Viewerの初期化
void InitializeViewer(const std::string &windowTitle,
                      Handle(V3d_Viewer) & viewer,
                      Handle(V3d_View) & view,
                      Handle(AIS_InteractiveContext) & context)
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

    // `context` と `view` をウィンドウに関連付け
    ViewerData *viewerData = new ViewerData{context, view};
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(viewerData));
}

// 座標系を表示
void DisplayViewCube(const Handle(AIS_InteractiveContext) & context, const Handle(V3d_View) & view)
{
    Handle(AIS_ViewCube) viewCube = new AIS_ViewCube();
    viewCube->SetSize(50.0);
    viewCube->SetBoxColor(Quantity_NOC_GRAY);
    viewCube->SetDrawAxes(Standard_True);
    viewCube->SetTransparency(0.5);
    viewCube->SetZLayer(Graphic3d_ZLayerId_Topmost);

    Graphic3d_Vec2i offset(100, 100);
    viewCube->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers, Aspect_TOTP_RIGHT_LOWER, offset));

    Handle(AIS_AnimationCamera) animation = new AIS_AnimationCamera("ViewCubeAnimation", view);
    viewCube->SetViewAnimation(animation);

    context->Display(viewCube, Standard_True);
}

// 楕円体を作成して表示
void DisplayEllipsoid(const Handle(AIS_InteractiveContext) & context,
                      const float radiusX, const float radiusY, const float radiusZ)
{
    gp_Ax2 ellipseAxis(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
    Handle(Geom_Ellipse) ellipse = new Geom_Ellipse(ellipseAxis, radiusX, radiusY);
    Handle(Geom_TrimmedCurve) trimmedEllipse = new Geom_TrimmedCurve(ellipse, 0, M_PI * 2);
    TopoDS_Edge ellipseEdge = BRepBuilderAPI_MakeEdge(trimmedEllipse);
    TopoDS_Shape ellipsoid = BRepPrimAPI_MakeRevol(ellipseEdge, ellipseAxis.Axis());

    Handle(AIS_Shape) aisEllipsoid = new AIS_Shape(ellipsoid);

    Handle(Prs3d_ShadingAspect) shadingAspect = new Prs3d_ShadingAspect();
    shadingAspect->SetTransparency(0.7);
    shadingAspect->SetColor(Quantity_NOC_BLUE1);
    aisEllipsoid->Attributes()->SetShadingAspect(shadingAspect);

    context->Display(aisEllipsoid, Standard_True);
}

// 点群を表示
void DisplayPointCloud(const Handle(AIS_InteractiveContext) & context,
                       const std::vector<std::array<float, 3>> &points)
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
}