#include "ViewerWidget.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <AIS_Shape.hxx>
#include <STEPControl_Reader.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDF_Label.hxx>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFile>
#include <QMessageBox>
#include <iostream>

ViewerWidget::ViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    // OpenCASCADE の初期化
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    // ビューアとビューの作成
    m_viewer = new V3d_Viewer(graphicDriver);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_view = m_viewer->CreateView();
    Handle(WNT_Window) wntWindow = new WNT_Window((Aspect_Handle)winId());
    m_view->SetWindow(wntWindow);

    if (!wntWindow->IsMapped())
    {
        wntWindow->Map();
    }

    // 描画コンテキストの作成
    m_context = new AIS_InteractiveContext(m_viewer);

    m_view->FitAll();
}

ViewerWidget::~ViewerWidget() {}

void ViewerWidget::loadStepFile(const char* filePath)
{
    // STEP ファイルを読み込む
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filePath);
    if (status != IFSelect_RetDone)
    {
        std::cerr << "Failed to read STEP file: " << filePath << std::endl;
        return;
    }

    // STEP ファイルから形状を取得
    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();

    if (shape.IsNull())
    {
        std::cerr << "No valid shape found in STEP file: " << filePath << std::endl;
        return;
    }

    // 形状を表示
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    m_context->Display(aisShape, Standard_True);
    m_view->FitAll();
}

void ViewerWidget::displayShape(const TopoDS_Shape &shape, double transparency, const Quantity_Color *faceColor, const Quantity_Color *edgeColor)
{
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);

    // フェイスの色を設定（指定がある場合のみ）
    if (faceColor)
    {
        m_context->SetColor(aisShape, *faceColor, AIS_Shaded);
    }

    // エッジの色を設定（指定がある場合のみ）
    if (edgeColor)
    {
        m_context->SetColor(aisShape, *edgeColor, AIS_WireFrame);
    }

    // 透過度を設定 (0.0 = 不透明, 1.0 = 完全に透明)
    aisShape->SetTransparency(transparency);

    // シェーディングモードで表示
    m_context->SetDisplayMode(aisShape, AIS_Shaded, Standard_True);

    // 形状を表示
    m_context->Display(aisShape, Standard_True);

    m_view->FitAll();
}

void ViewerWidget::paintEvent(QPaintEvent *event)
{
    if (!m_view.IsNull())
    {
        m_view->Redraw();
    }
}

void ViewerWidget::resizeEvent(QResizeEvent *event)
{
    if (!m_view.IsNull())
    {
        m_view->MustBeResized();
    }
}

void ViewerWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();
    if (event->buttons() & Qt::LeftButton)
    {
        m_view->StartRotation(m_lastMousePos.x(), m_lastMousePos.y());
    }
}

void ViewerWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    if (event->buttons() & Qt::LeftButton)
    {
        m_view->Rotation(currentPos.x(), currentPos.y());
    }
    else if (event->buttons() & Qt::RightButton)
    {
        int dx = currentPos.x() - m_lastMousePos.x();
        int dy = currentPos.y() - m_lastMousePos.y();
        m_view->Pan(dx, dy);
    }
    m_lastMousePos = currentPos;
}

void ViewerWidget::wheelEvent(QWheelEvent *event)
{
    if (!m_view.IsNull())
    {
        int delta = event->angleDelta().y();
        if (delta > 0)
        {
            m_view->SetZoom(0.9); // ズームイン
        }
        else
        {
            m_view->SetZoom(1.1); // ズームアウト
        }
    }
}