#define _WIN32_WINNT 0x0601 // Target Windows 7 or later
#include <windows.h>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <WNT_Window.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
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

int main()
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
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0, className, L"OpenCASCADE Viewer", // Unicode 文字列に変更
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
    {
        std::cerr << "Failed to create window." << std::endl;
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);

    // OpenCASCADE の初期化
    std::cout << "Program started." << std::endl;

    // OpenCASCADEのログをファイルにリダイレクト
    OSD_Environment logEnvOut("CSF_DefaultStdOut", "");
    OSD_Environment logEnvErr("CSF_DefaultStdErr", "");

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

    Graphic3d_RenderingParams &params = view->ChangeRenderingParams();
    params.IsTransparentShadowEnabled = Standard_True; // 透過影を有効化

    // シンプルな形状（ボックス）を作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();
    Handle(AIS_Shape) aisBox = new AIS_Shape(box);

    // 表示属性を設定
    Handle(Prs3d_ShadingAspect) shadingAspect = new Prs3d_ShadingAspect();
    shadingAspect->SetTransparency(0.5);         // 透過率（0.0 = 不透明、1.0 = 完全透明）
    shadingAspect->SetColor(Quantity_NOC_BLUE1); // ボックスの色
    aisBox->Attributes()->SetShadingAspect(shadingAspect);

    // 表面とエッジを表示
    context->SetDisplayMode(aisBox, AIS_Shaded, Standard_False);   // 表面を表示
    context->SetDisplayMode(aisBox, AIS_WireFrame, Standard_True); // エッジを表示

    // ボックスを表示
    context->Display(aisBox, Standard_True);

    view->FitAll();

    // ボックスをファイルに保存
    if (BRepTools::Write(box, "box.brep"))
    {
        std::cout << "Box shape saved to 'box.brep'" << std::endl;
    }
    else
    {
        std::cerr << "Failed to save box shape" << std::endl;
    }

    // STEPファイルライターを作成
    STEPControl_Writer stepWriter;

    // 形状をSTEPファイルに追加
    IFSelect_ReturnStatus status = stepWriter.Transfer(box, STEPControl_AsIs);
    if (status != IFSelect_RetDone)
    {
        std::cerr << "Error: Failed to transfer shape to STEP writer." << std::endl;
        return 1;
    }

    // STEPファイルに書き出し
    Standard_CString stepFileName = "box.stp";
    status = stepWriter.Write(stepFileName);
    if (status != IFSelect_RetDone)
    {
        std::cerr << "Error: Failed to write STEP file." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "STEP file written successfully." << std::endl;
        std::cout << "STEP file successfully written to " << stepFileName << std::endl;
    }

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}