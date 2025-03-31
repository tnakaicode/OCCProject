#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
#include <iostream>

int main() {
    std::cout << "OpenCASCADE Example" << std::endl;

    // ボックスを作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();

    // ボックスをファイルに保存
    if (BRepTools::Write(box, "box.brep")) {
        std::cout << "Box shape saved to 'box.brep'" << std::endl;
    } else {
        std::cerr << "Failed to save box shape" << std::endl;
    }

    return 0;
}