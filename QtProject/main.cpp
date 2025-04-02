#include "ViewerWidget.h"
#include <QApplication>
#include <BRepPrimAPI_MakeBox.hxx> // OpenCASCADE のボックス作成用クラス

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ViewerWidget viewer;
    viewer.resize(800, 600);
    viewer.show();

    // OpenCASCADE でボックスを作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();

    // ViewerWidget にボックスを表示
    viewer.displayShape(box);

    return app.exec();
}