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
#include <FairCurve_EnergyOfMVC.hxx>
#include <FairCurve_BattenLaw.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <TColgp_HArray1OfPnt2d.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <AIS_Shape.hxx>

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

    //! 関数値と勾配を計算
    virtual Standard_Boolean Values(const math_Vector &X, Standard_Real &F, math_Vector &G) override
    {
        // 関数値
        Value(X, F);

        // 勾配
        Gradient(X, G);

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

    //! 次元数を返す
    virtual Standard_Integer NbVariables() const override
    {
        return 2; // 2次元 (x, y)
    }
};

int main()
{

    // Viewerを初期化
    Handle(V3d_Viewer) viewer;
    Handle(V3d_View) view;
    Handle(AIS_InteractiveContext) context;
    InitializeViewer("Point Cloud Viewer", viewer, view, context);
    DisplayViewCube(context, view);

    // 最適化対象の関数を作成
    MyFunction function;

    // 初期値を設定 (例: x = 0, y = 0)
    math_Vector startPoint(1, 2); // 2次元ベクトル
    startPoint(1) = 0.0;          // x
    startPoint(2) = 0.0;          // y

    // FairCurve_Newton を初期化
    FairCurve_Newton optimizer(function, 1.0e-7, 1.0e-7, 40, 1.0e-6, Standard_True);

    // 最適化を実行
    optimizer.Perform(function, startPoint);

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

    // Bスプラインの次数
    Standard_Integer BSplOrder = 3;

    // 制御点を定義
    TColgp_Array1OfPnt poles(1, 4);
    poles.SetValue(1, gp_Pnt(0.0, 0.0, 0.0));
    poles.SetValue(2, gp_Pnt(2.0, 1.0, 0.0));
    poles.SetValue(3, gp_Pnt(5.0, 2.0, 0.0));
    poles.SetValue(4, gp_Pnt(1.0, 3.0, 0.0));

    // ノットベクトルを定義
    TColStd_Array1OfReal knots(1, 6);
    knots.SetValue(1, 0.0);
    knots.SetValue(2, 0.01);
    knots.SetValue(3, 0.5);
    knots.SetValue(4, 0.75);
    knots.SetValue(5, 0.99);
    knots.SetValue(6, 1.0);

    // マルチシティを定義
    TColStd_Array1OfInteger multiplicities(1, 6); // ノットベクトルのサイズに合わせる
    multiplicities.SetValue(1, 2);                // 最初のノットのマルチシティ（次数に一致）
    multiplicities.SetValue(2, 1);                // 中間のノット
    multiplicities.SetValue(3, 1);                // 中間のノット
    multiplicities.SetValue(4, 1);                // 中間のノット
    multiplicities.SetValue(5, 1);                // 中間のノット
    multiplicities.SetValue(6, 2);                // 最後のノットのマルチシティ（次数に一致）

    // バテン法則を定義
    Standard_Real Heigth = 1.0;  // 中央点の高さ
    Standard_Real Slope = 0.5;   // 幾何学的な傾斜
    Standard_Real Sliding = 2.0; // 有効長さ

    FairCurve_BattenLaw battenLaw(Heigth, Slope, Sliding);

    // その他のパラメータ
    Standard_Integer ContrOrder1 = 2; // 1つ目の制約次数
    Standard_Integer ContrOrder2 = 2; // 2つ目の制約次数
    Standard_Real PhysicalRatio = 1.0;
    Standard_Real LengthSliding = 0.5;
    Standard_Boolean FreeSliding = Standard_False; // スライディングを固定
    Standard_Real Angle1 = 0.0;
    Standard_Real Angle2 = 0.0;
    Standard_Real Curvature1 = 0.0;
    Standard_Real Curvature2 = 0.0;

    // poles を TColgp_HArray1OfPnt2d に変換
    Handle(TColgp_HArray1OfPnt2d) poles2d = new TColgp_HArray1OfPnt2d(1, poles.Length());
    for (Standard_Integer i = poles.Lower(); i <= poles.Upper(); ++i)
    {
        const gp_Pnt &point = poles.Value(i);
        poles2d->SetValue(i, gp_Pnt2d(point.X(), point.Y())); // Z座標を無視して2Dに変換
    }

    // ノットベクトルを定義
    Handle(TColStd_HArray1OfReal) hknots = new TColStd_HArray1OfReal(1, knots.Length());
    for (Standard_Integer i = knots.Lower(); i <= knots.Upper(); ++i)
    {
        hknots->SetValue(i, knots.Value(i));
    }

    // FairCurve_EnergyOfMVC を初期化
    FairCurve_EnergyOfMVC energyOfMVC(
        BSplOrder,
        hknots,
        poles2d,
        ContrOrder1,
        ContrOrder2,
        battenLaw,
        PhysicalRatio,
        LengthSliding,
        FreeSliding,
        Angle1,
        Angle2,
        Curvature1,
        Curvature2);

    // エネルギーのスライディング長さを取得
    Standard_Real lengthSliding = energyOfMVC.LengthSliding();
    std::cout << "Length Sliding: " << lengthSliding << std::endl;

    // 状態を取得
    FairCurve_AnalysisCode status = energyOfMVC.Status();
    std::cout << "Status: " << static_cast<int>(status) << std::endl;

    // 変数を計算
    math_Vector variables(1, poles.Length());
    if (energyOfMVC.Variable(variables))
    {
        std::cout << "Variables: " << variables.Length() << std::endl;
        for (Standard_Integer i = 1; i <= variables.Length(); ++i)
        {
            std::cout << i << " : " << variables(i) << " / " << poles.Value(i).X() << std::endl;
        }
        std::cout << std::endl;
    }
    else
    {
        std::cerr << "Failed to compute variables." << std::endl;
    }

    TColgp_Array1OfPnt newPoles(1, poles.Length());
    for (Standard_Integer i = 1; i <= variables.Length(); ++i)
    {
        const gp_Pnt &originalPoint = poles.Value(i);
        newPoles.SetValue(i, gp_Pnt(variables(i), originalPoint.Y(), originalPoint.Z())); // 必要に応じて座標を調整
    }

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
    // DisplayEllipsoid(context, radiusX, radiusY, radiusZ);

    // 点群を表示し、AISオブジェクトを取得
    // Handle(AIS_PointCloud) originalPointCloud = DisplayPointCloud(context, ellipsePoints);

    // 別の座標変換を適用して点群を移動
    gp_Trsf secondTransform;
    secondTransform.SetTranslation(gp_Vec(5.0, 0.0, 0.0)); // X方向に5.0移動

    // originalPointCloudに座標変換を適用
    // originalPointCloud->SetLocalTransformation(secondTransform);
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
        // DisplayPoint(context, vertex);
    }

    std::cout << "Point Cloud Processing Completed." << std::endl;

    // Bスプライン曲線を表示
    // 色を指定
    Quantity_Color redColor(Quantity_NOC_RED1);
    Quantity_Color bluColor(Quantity_NOC_BLUE1);

    DisplayBSplineCurve(context, poles, knots, multiplicities, 3, &redColor);
    DisplayBSplineCurve(context, newPoles, knots, multiplicities, 3, &bluColor);

    std::cout << "B-Spline Curve created and displayed." << std::endl;

    view->FitAll();
    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}