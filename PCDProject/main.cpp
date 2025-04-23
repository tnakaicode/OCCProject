#define _WIN32_WINNT 0x0601 // Target Windows 7 or later
#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include "Viewer.h"
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Quaternion.hxx>
#include <FEmtool_Curve.hxx>
#include <FairCurve_Newton.hxx>
#include <math_MultipleVarFunctionWithHessian.hxx>

// 最適化対象の関数を定義
class MyFunction : public math_MultipleVarFunctionWithHessian
{
public:
    //! コンストラクタ
    MyFunction() {}

    //! 関数値を計算
    virtual Standard_Boolean Value(const math_Vector &X, Standard_Real &F) override
    {
        // 例: f(x, y) = (x - 1)^2 + (y - 2)^2
        F = pow(X(1) - 1.0, 2) + pow(X(2) - 2.0, 2);
        return Standard_True;
    }

    //! 勾配を計算
    virtual Standard_Boolean Gradient(const math_Vector &X, math_Vector &G) override
    {
        // 例: ∇f(x, y) = [2(x - 1), 2(y - 2)]
        G(1) = 2.0 * (X(1) - 1.0);
        G(2) = 2.0 * (X(2) - 2.0);
        return Standard_True;
    }

    //! ヘッセ行列を計算
    virtual Standard_Boolean Values(const math_Vector &X, Standard_Real &F, math_Vector &G, math_Matrix &H) override
    {
        // 関数値
        Value(X, F);

        // 勾配
        Gradient(X, G);

        // ヘッセ行列 (2x2 の定数行列)
        H(1, 1) = 2.0;
        H(1, 2) = 0.0;
        H(2, 1) = 0.0;
        H(2, 2) = 2.0;

        return Standard_True;
    }
};
int main()
{
    // 最適化対象の関数を作成
    MyFunction function;

    // 初期値を設定 (例: x = 0, y = 0)
    math_Vector startPoint(1, 2); // 2次元ベクトル
    startPoint(1) = 0.0;          // x
    startPoint(2) = 0.0;          // y

    // FairCurve_Newton を初期化
    FairCurve_Newton optimizer(function, 1.0e-7, 1.0e-7, 40, 1.0e-6, Standard_True);

    // 最適化を実行
    optimizer.Perform(startPoint);

    // 結果を取得
    if (optimizer.IsDone())
    {
        const math_Vector &solution = optimizer.Location();
        std::cout << "Optimization completed successfully!" << std::endl;
        std::cout << "Optimal solution: x = " << solution(1) << ", y = " << solution(2) << std::endl;
    }
    else
    {
        std::cout << "Optimization failed." << std::endl;
    }

    // Viewerを初期化
    Handle(V3d_Viewer) viewer;
    Handle(V3d_View) view;
    Handle(AIS_InteractiveContext) context;
    InitializeViewer("Point Cloud Viewer", viewer, view, context);
    DisplayViewCube(context, view);

    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 新しい座標系を定義（例: 平行移動 + 回転）
    gp_Pnt origin(1.0, 2.0, 0.5); // 新しい座標系の原点
    gp_Dir zDir(0.0, 0.0, 1.0);   // Z軸方向
    gp_Dir xDir(1.0, 0.0, 0.0);   // X軸方向
    gp_Ax3 newAxis(origin, zDir, xDir);

    // 座標変換行列を取得
    gp_Trsf transform;
    transform.SetTransformation(newAxis);

    // 楕円の点群データを生成
    std::vector<gp_Pnt> ellipsePoints;
    const int numPoints = 10'000'000; // 点の数
    const float radiusX = 2.0f;       // 長軸
    const float radiusY = 1.0f;       // 短軸
    const float radiusZ = 1.5f;       // Z軸方向のスケール

    for (int i = 0; i < numPoints; ++i)
    {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * static_cast<float>(M_PI); // 0 to 2π
        float phi = acos(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f);                 // 0 to π
        float x = radiusX * sin(phi) * cos(theta);
        float y = radiusY * sin(phi) * sin(theta);
        float z = radiusZ * cos(phi);
        gp_Pnt point(x, y, z);
        point.Transform(transform);     // 座標変換を適用
        ellipsePoints.push_back(point); // 座標変換を適用
    }

    // 楕円体を表示
    DisplayEllipsoid(context, radiusX, radiusY, radiusZ);

    // 点群を表示し、AISオブジェクトを取得
    Handle(AIS_PointCloud) originalPointCloud = DisplayPointCloud(context, ellipsePoints);

    // 別の座標変換を適用して点群を移動
    gp_Trsf secondTransform;
    secondTransform.SetTranslation(gp_Vec(5.0, 0.0, 0.0)); // X方向に5.0移動

    // originalPointCloudに座標変換を適用
    originalPointCloud->SetLocalTransformation(secondTransform);
    // context->Display(originalPointCloud, Standard_True);

    // 楕円体を覆う長方形の頂点を計算
    std::vector<gp_Pnt> boundingBoxVertices = {
        gp_Pnt(-radiusX, -radiusY, -radiusZ), // 頂点1
        gp_Pnt(radiusX, -radiusY, -radiusZ),  // 頂点2
        gp_Pnt(radiusX, radiusY, -radiusZ),   // 頂点3
        gp_Pnt(-radiusX, radiusY, -radiusZ),  // 頂点4
        gp_Pnt(-radiusX, -radiusY, radiusZ),  // 頂点5
        gp_Pnt(radiusX, -radiusY, radiusZ),   // 頂点6
        gp_Pnt(radiusX, radiusY, radiusZ),    // 頂点7
        gp_Pnt(-radiusX, radiusY, radiusZ)    // 頂点8
    };

    // 変換を適用した頂点を描画
    for (auto &vertex : boundingBoxVertices)
    {
        vertex.Transform(transform); // 座標変換を適用
        DisplayPoint(context, vertex);
    }
    view->FitAll(); // 全体を表示

    std::cout << "Point Cloud Processing Completed." << std::endl;

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}