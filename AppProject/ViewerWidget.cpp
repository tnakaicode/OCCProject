#include "ViewerWidget.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>

ViewerWidget::ViewerWidget(QWidget *parent)
    : QWidget(parent) {
    initializeOpenCASCADE();
}

ViewerWidget::~ViewerWidget() {}

void ViewerWidget::initializeOpenCASCADE() {
    // OpenCASCADEの初期化
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(Graphic3d_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);
    Handle(V3d_Viewer) viewer = new V3d_Viewer(graphicDriver);
    m_view = viewer->CreateView();
    m_context = new AIS_InteractiveContext(viewer);

    // ボックスを作成して表示
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 50.0, 30.0).Shape();
    Handle(AIS_Shape) aisBox = new AIS_Shape(box);
    m_context->Display(aisBox, Standard_True);
}

void ViewerWidget::paintEvent(QPaintEvent *event) {
    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void ViewerWidget::resizeEvent(QResizeEvent *event) {
    if (!m_view.IsNull()) {
        m_view->MustBeResized();
    }
}