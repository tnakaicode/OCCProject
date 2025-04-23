#ifndef VIEWER_H
#define VIEWER_H

#include <windows.h>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <WNT_Window.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_PointCloud.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <gp_Pnt.hxx>
#include <vector>
#include <array>

// ウィンドウのサイズと位置を保存
void SaveWindowConfig(HWND hwnd);

// ウィンドウのサイズと位置を読み込み
void LoadWindowConfig(HWND hwnd);

// Viewerの初期化
void InitializeViewer(const std::string &windowTitle,
                      Handle(V3d_Viewer) & viewer,
                      Handle(V3d_View) & view,
                      Handle(AIS_InteractiveContext) & context);

// 座標系を表示
void DisplayViewCube(const Handle(AIS_InteractiveContext) & context, const Handle(V3d_View) & view);

// 楕円体を作成して表示
void DisplayEllipsoid(const Handle(AIS_InteractiveContext) & context,
                      const float radiusX, const float radiusY, const float radiusZ);

// 点群を表示
Handle(AIS_PointCloud) DisplayPointCloud(const Handle(AIS_InteractiveContext) & context,
                                         const std::vector<gp_Pnt> &points);

void DisplayPoint(const Handle(AIS_InteractiveContext) & context, const gp_Pnt &point);

// Bスプライン曲線を表示
void DisplayBSplineCurve(const Handle(AIS_InteractiveContext) & context,
                         const TColgp_Array1OfPnt &controlPoints,
                         const TColStd_Array1OfReal &knots,
                         const TColStd_Array1OfInteger &multiplicities,
                         Standard_Integer degree);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // VIEWER_H