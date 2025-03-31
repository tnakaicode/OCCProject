#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <gp_Pnt.hxx>
#include <iostream>

int main() {
    try {
        // Windows用のディスプレイ接続を作成
        Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();

        // OpenGLグラフィックドライバを作成
        Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

        // 3Dビューアを作成
        Handle(V3d_Viewer) viewer = new V3d_Viewer(graphicDriver);
        viewer->SetDefaultLights();
        viewer->SetLightOn();

        // Windowsネイティブウィンドウを作成
        Handle(WNT_Window) window = new WNT_Window("OpenCASCADE Viewer", 100, 100, 800, 600);

        // 3Dビューを作成
        Handle(V3d_View) view = viewer->CreateView();
        view->SetWindow(window);
        if (!window->IsMapped()) {
            window->Map();
        }

        // インタラクティブコンテキストを作成
        Handle(AIS_InteractiveContext) context = new AIS_InteractiveContext(viewer);

        // シンプルな形状（ボックス）を作成
        TopoDS_Shape box = BRepPrimAPI_MakeBox(gp_Pnt(0, 0, 0), gp_Pnt(100, 100, 100)).Shape();
        Handle(AIS_Shape) aisBox = new AIS_Shape(box);
        context->Display(aisBox, AIS_Shaded, 0, false);

        // 3Dビューを初期化
        view->FitAll();

        // メインループ（簡易版）
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
    } catch (Standard_Failure& failure) {
        std::cerr << "Error: " << failure.GetMessageString() << std::endl;
        return 1;
    }

    return 0;
}