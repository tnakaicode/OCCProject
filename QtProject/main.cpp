#include "ViewerWidget.h"
#include <QApplication>
#include <BRepPrimAPI_MakeBox.hxx> // OpenCASCADE のボックス作成用クラス
#include <direct.h> // Windows 環境でカレントディレクトリを取得するためのヘッダー
#include <iostream>

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
    viewer.displayShape(box, 0.5, &faceColor, nullptr); // 透過度 0.5

    // カレントディレクトリを取得
    char currentDir[FILENAME_MAX];
    if (_getcwd(currentDir, sizeof(currentDir)) == nullptr) {
        std::cerr << "Failed to get current directory." << std::endl;
        return -1;
    }
    currentDir[sizeof(currentDir) - 1] = '\0'; // 念のためヌル終端を保証
    std::cout << "Current Directory: " << currentDir << std::endl;

    // STEP ファイルのパスを設定
    std::string stepFilePath = std::string(currentDir) + "/as1-oc-214.stp";
    std::cout << "Step File Path: " << stepFilePath << std::endl;

    // STEP ファイルを読み込む
    viewer.loadStepFile(stepFilePath.c_str());

    return app.exec();
}