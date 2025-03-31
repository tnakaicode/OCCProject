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
#include <gp_Pnt.hxx>
#include <Quantity_NameOfColor.hxx>
#include <OSD_Environment.hxx>
#include <iostream>

int main()
{

    // OpenCASCADEのログをファイルにリダイレクト
    OSD_Environment logEnvOut("CSF_DefaultStdOut", "");
    OSD_Environment logEnvErr("CSF_DefaultStdErr", "");

    // シンプルな形状（ボックス）を作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();
    // ボックスをファイルに保存
    if (BRepTools::Write(box, "box.brep"))
    {
        std::cout << "Box shape saved to 'box.brep'" << std::endl;
    }
    else
    {
        std::cerr << "Failed to save box shape" << std::endl;
    }

    return 0;
}