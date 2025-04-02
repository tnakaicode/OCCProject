#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QWidget>
#include <Aspect_Handle.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_Color.hxx>

class ViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *parent = nullptr);
    ~ViewerWidget();

    // ボックスや他の形状を表示するためのメソッド
    void displayShape(const TopoDS_Shape &shape, double transparency = 0.0, const Quantity_Color *faceColor = nullptr, const Quantity_Color *edgeColor = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;                  // OpenCASCADE のビュー
    Handle(AIS_InteractiveContext) m_context; // OpenCASCADE の描画コンテキスト
    QPoint m_lastMousePos;                    // マウスの最後の位置
};

#endif // VIEWERWIDGET_H