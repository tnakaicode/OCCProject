#include <iostream>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
#include <OSD_Environment.hxx>
#include <gp_Pnt.hxx>

int main() {
    // OpenCASCADEのログをファイルにリダイレクト
    OSD_Environment logEnvOut("CSF_DefaultStdOut", "log.txt");
    OSD_Environment logEnvErr("CSF_DefaultStdErr", "error_log.txt");

    std::cout << "OpenCASCADE Example" << std::endl;

    // 点を作成
    gp_Pnt point(1.0, 2.0, 3.0);
    std::cout << "Point coordinates: (" << point.X() << ", " << point.Y() << ", " << point.Z() << ")" << std::endl;

    // ボックスを作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();
    std::cout << "After creating the box" << std::endl;

    // ボックスをファイルに保存
    if (BRepTools::Write(box, "box.brep")) {
        std::cout << "Box shape saved to 'box.brep'" << std::endl;
    } else {
        std::cerr << "Failed to save box shape" << std::endl;
    }

    return 0;
}