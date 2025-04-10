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
#include <gp_Pnt.hxx>
#include <vector>
#include <array>

// グローバル変数
extern Handle(V3d_Viewer) viewer;
extern Handle(V3d_View) view;
extern Handle(AIS_InteractiveContext) context;

// Viewerの初期化
void InitializeViewer(const std::string &windowTitle);

// 座標系を表示
void DisplayViewCube();

// 楕円体を作成して表示
void DisplayEllipsoid(const Handle(AIS_InteractiveContext) & context,
                      const float radiusX, const float radiusY, const float radiusZ);

// 点群を表示
void DisplayPointCloud(const Handle(AIS_InteractiveContext) & context,
                       const std::vector<std::array<float, 3>> &points);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // VIEWER_H