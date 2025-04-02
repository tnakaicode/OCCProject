#include "ViewerWidget.h"
#include <QApplication>
#include <BRepPrimAPI_MakeBox.hxx> // OpenCASCADE のボックス作成用クラス

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ViewerWidget viewer;
    viewer.resize(800, 600);
    viewer.show();

    // OpenCASCADE でボックスを作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();

    // 色を指定
    Quantity_Color faceColor(Quantity_NOC_BLUE1); // フェイスの色: 青
    Quantity_Color edgeColor(Quantity_NOC_BLACK); // エッジの色: 黒

    // ViewerWidget にボックスを表示
    viewer.displayShape(box, 0.5, &faceColor, nullptr); // 透過度 0.0 (不透明)

    return app.exec();
}