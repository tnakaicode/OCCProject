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
#include <iostream>

// グローバル変数
Handle(V3d_Viewer) viewer;
Handle(V3d_View) view;
Handle(AIS_InteractiveContext) context;

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT:
        if (!view.IsNull()) {
            view->Redraw();
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
    const wchar_t* className = L"OpenCASCADEViewer"; // Unicode 文字列に変更

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClass(&wc)) {
        std::cerr << "Failed to register window class." << std::endl;
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0, className, L"OpenCASCADE Viewer", // Unicode 文字列に変更
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) {
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

    if (!wntWindow->IsMapped()) {
        wntWindow->Map();
    }

    context = new AIS_InteractiveContext(viewer);

    // シンプルな形状（ボックス）を作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();
    Handle(AIS_Shape) aisBox = new AIS_Shape(box);
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
    if (status != IFSelect_RetDone) {
        std::cerr << "Error: Failed to transfer shape to STEP writer." << std::endl;
        return 1;
    }

    // STEPファイルに書き出し
    Standard_CString stepFileName = "box.stp";
    status = stepWriter.Write(stepFileName);
    if (status != IFSelect_RetDone) {
        std::cerr << "Error: Failed to write STEP file." << std::endl;
        return 1;
    }

    std::cout << "STEP file successfully written to " << stepFileName << std::endl;

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}