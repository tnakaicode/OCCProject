#include "Viewer.h"
#include <AIS_ViewCube.hxx>
#include <AIS_AnimationCamera.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_PointCloud.hxx>
#include <AIS_Point.hxx>
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
#include <Geom_CartesianPoint.hxx>
#include <Geom_BSplineCurve.hxx>
#include <STEPControl_Writer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Quantity_NameOfColor.hxx>
#include <OSD_Environment.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

// 設定ファイルのパス
const std::string CONFIG_FILE = "viewer_config.txt";

// アプリのバージョン情報を定義
#ifdef _DEBUG
const std::string APP_VERSION = "Version 1.0.0 - Debug";
#else
const std::string APP_VERSION = "Version 1.0.0 - Release";
#endif

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

void PrintGraphicDriverInfo(const Handle(OpenGl_GraphicDriver) & graphicDriver)
{
    if (!graphicDriver.IsNull())
    {
        std::cout << "GraphicDriver Information:" << std::endl;

        // OpenGL コンテキスト情報を取得
        const Handle(OpenGl_Context) &glContext = graphicDriver->GetSharedContext();
        if (!glContext.IsNull())
        {
            std::cout << "  OpenGL Version: " << glContext->VersionMajor() << "." << glContext->VersionMinor() << std::endl;
            std::cout << "  Vendor: " << glContext->Vendor() << std::endl;
            std::cout << "  Renderer: " << glContext->RenderingContext() << std::endl;
            // std::cout << "  GLSL Version: " << glContext->GLSLVersion() << std::endl;
            std::cout << "  Max Texture Size: " << glContext->MaxTextureSize() << std::endl;
            std::cout << "  Max Combined Texture Units: " << glContext->MaxCombinedTextureUnits() << std::endl;
            // std::cout << "  Max Samples: " << glContext->MaxSamples() << std::endl;
        }
        else
        {
            std::cout << "  OpenGL Context: Not available" << std::endl;
        }

        // メモリ情報を取得
        Standard_Size freeMemory = 0;
        TCollection_AsciiString memoryInfo;
        if (graphicDriver->MemoryInfo(freeMemory, memoryInfo))
        {
            std::cout << "  Free GPU Memory: " << freeMemory << " bytes" << std::endl;
            std::cout << "  Memory Info: " << memoryInfo.ToCString() << std::endl;
        }
        else
        {
            std::cout << "  Memory Info: Not available" << std::endl;
        }

        // VSync 情報を取得
        bool isVSyncEnabled = graphicDriver->IsVerticalSync();
        std::cout << "  Vertical Sync: " << (isVSyncEnabled ? "Enabled" : "Disabled") << std::endl;

        // その他の情報を取得
        const OpenGl_Caps &caps = graphicDriver->Options();
        std::cout << "  Double Buffer: " << (caps.buffersNoSwap ? "Disabled" : "Enabled") << std::endl;
        // std::cout << "  Stereo: " << (caps.stereo ? "Enabled" : "Disabled") << std::endl;
        // std::cout << "  MSAA Samples: " << caps.msaaSamples << std::endl;
        // std::cout << "  Max Clip Planes: " << caps.maxClipPlanes << std::endl;
        // std::cout << "  Max Lights: " << caps.maxLights << std::endl;
    }
    else
    {
        std::cout << "GraphicDriver is null." << std::endl;
    }
}

void PrintContextInfo(const Handle(AIS_InteractiveContext) & context)
{
    if (!context.IsNull())
    {
        std::cout << "AIS_InteractiveContext Information:" << std::endl;

        // 表示されているオブジェクト数を取得
        AIS_ListOfInteractive displayedObjects;
        context->DisplayedObjects(displayedObjects);
        int displayedCount = displayedObjects.Extent();
        std::cout << "  Number of Displayed Objects: " << displayedCount << std::endl;

        // 選択されているオブジェクト数を取得
        int selectedCount = context->NbSelected();
        std::cout << "  Number of Selected Objects: " << selectedCount << std::endl;

        // ハイライトされているオブジェクト数を取得
        AIS_ListOfInteractive highlightedObjects;
        // context->ObjectsByDisplayStatus(PrsMgr_DisplayStatus_Hilighted, highlightedObjects);
        // int highlightedCount = highlightedObjects.Extent();
        // std::cout << "  Number of Highlighted Objects: " << highlightedCount << std::endl;
    }
    else
    {
        std::cout << "AIS_InteractiveContext is null." << std::endl;
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

    static bool isDragging = false;
    static bool isPanning = false;
    static int lastX = 0, lastY = 0;

    // 一度だけ viewerData を取得してローカル変数にキャッシュ
    ViewerData *viewerData = reinterpret_cast<ViewerData *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    Handle(AIS_InteractiveContext) context = viewerData ? viewerData->context : nullptr;
    Handle(V3d_View) view = viewerData ? viewerData->view : nullptr;

    switch (msg)
    {
    case WM_CREATE:
    {
        // ウィンドウ作成時に設定ファイルからサイズと位置を読み込む
        LoadWindowConfig(hwnd);

        // ウィンドウタイトルにバージョン情報を設定
        std::string windowTitle = "Point Cloud Viewer - " + APP_VERSION;
        SetWindowTextA(hwnd, windowTitle.c_str());
    }
    break;

    case WM_LBUTTONDOWN:
    {
        if (context && view)
        {
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
            else
            {
                isDragging = true;
                lastX = LOWORD(lParam);
                lastY = HIWORD(lParam);
                view->StartRotation(lastX, lastY);
            }
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
            view->Pan(static_cast<Standard_Integer>(dx), static_cast<Standard_Integer>(dy));
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

    HWND hwnd = CreateWindowEx(0, className,
                               std::wstring(windowTitle.begin(), windowTitle.end()).c_str(),
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
    context = new AIS_InteractiveContext(viewer);

    // OpenGL コンテキストの初期化を確認
    if (!graphicDriver->InitContext())
    {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return;
    }

    if (context.IsNull())
    {
        std::cerr << "Failed to initialize AIS_InteractiveContext." << std::endl;
        return;
    }

    PrintGraphicDriverInfo(graphicDriver);
    PrintContextInfo(context);

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

// 点群を表示し、AIS_PointCloudオブジェクトを返す
Handle(AIS_PointCloud) DisplayPointCloud(const Handle(AIS_InteractiveContext) & context,
                                         const std::vector<gp_Pnt> &points) // 修正: std::vector<gp_Pnt> を受け取る
{
    Handle(TColgp_HArray1OfPnt) pointArray = new TColgp_HArray1OfPnt(1, static_cast<Standard_Integer>(points.size()));
    for (Standard_Integer i = 0; i < points.size(); ++i)
    {
        pointArray->SetValue(i + 1, points[i]); // 修正: gp_Pnt を直接設定
    }

    Handle(AIS_PointCloud) aisPointCloud = new AIS_PointCloud();
    aisPointCloud->SetPoints(pointArray);

    context->Display(aisPointCloud, Standard_True);
    return aisPointCloud; // AIS_PointCloudオブジェクトを返す
}

// 単一の点を表示
void DisplayPoint(const Handle(AIS_InteractiveContext) & context, const gp_Pnt &point) // 修正: gp_Pnt を受け取る
{
    // 点を表す Prs3d_Point を作成
    Handle(Geom_CartesianPoint) geomPoint = new Geom_CartesianPoint(point);
    Handle(AIS_Point) aisPoint = new AIS_Point(geomPoint);

    // 点をコンテキストに追加
    context->Display(aisPoint, Standard_True);
}

// Bスプライン曲線を表示
void DisplayBSplineCurve(const Handle(AIS_InteractiveContext) & context,
                         const TColgp_Array1OfPnt &controlPoints,
                         const TColStd_Array1OfReal &knots,
                         const TColStd_Array1OfInteger &multiplicities,
                         Standard_Integer degree)
{
    try
    {
        // Bスプライン曲線を作成
        Handle(Geom_BSplineCurve) bsplineCurve = new Geom_BSplineCurve(controlPoints, knots, multiplicities, degree);

        // 曲線をエッジとして作成
        BRepBuilderAPI_MakeEdge edgeMaker(bsplineCurve);
        TopoDS_Edge edge = edgeMaker.Edge();

        // AIS_Shape を作成して表示
        Handle(AIS_Shape) aisCurve = new AIS_Shape(edge);
        context->Display(aisCurve, Standard_True);

        std::cout << "B-Spline curve displayed successfully." << std::endl;
    }
    catch (Standard_Failure &e)
    {
        std::cerr << "Error displaying B-Spline curve: " << e.GetMessageString() << std::endl;
    }
}
