#include "ViewerWidget.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

ViewerWidget::ViewerWidget(QWidget* parent)
    : QWidget(parent) {
    // OpenCASCADE の初期化
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    m_viewer = new V3d_Viewer(graphicDriver);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_view = m_viewer->CreateView();
    Handle(WNT_Window) wntWindow = new WNT_Window((Aspect_Handle)winId());
    m_view->SetWindow(wntWindow);

    if (!wntWindow->IsMapped()) {
        wntWindow->Map();
    }

    m_context = new AIS_InteractiveContext(m_viewer);

    // シンプルな形状（ボックス）を作成
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();
    Handle(AIS_Shape) aisBox = new AIS_Shape(box);
    m_context->Display(aisBox, Standard_True);

    m_view->FitAll();
}

ViewerWidget::~ViewerWidget() {}

void ViewerWidget::paintEvent(QPaintEvent* event) {
    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void ViewerWidget::resizeEvent(QResizeEvent* event) {
    if (!m_view.IsNull()) {
        m_view->MustBeResized();
    }
}

void ViewerWidget::mousePressEvent(QMouseEvent* event) {
    m_lastMousePos = event->pos();
    if (event->buttons() & Qt::LeftButton) {
        m_view->StartRotation(m_lastMousePos.x(), m_lastMousePos.y());
    }
}

void ViewerWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint currentPos = event->pos();
    if (event->buttons() & Qt::LeftButton) {
        m_view->Rotation(currentPos.x(), currentPos.y());
    } else if (event->buttons() & Qt::RightButton) {
        int dx = currentPos.x() - m_lastMousePos.x();
        int dy = currentPos.y() - m_lastMousePos.y();
        m_view->Pan(dx, dy);
    }
    m_lastMousePos = currentPos;
}

void ViewerWidget::wheelEvent(QWheelEvent* event) {
    if (!m_view.IsNull()) {
        int delta = event->angleDelta().y();
        if (delta > 0) {
            m_view->SetZoom(0.9); // ズームイン
        } else {
            m_view->SetZoom(1.1); // ズームアウト
        }
    }
}