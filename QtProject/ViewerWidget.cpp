#include "ViewerWidget.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <AIS_Shape.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
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
#include <iomanip>
#include <sstream>

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

void ViewerWidget::loadStepFile(const char *filePath)
{
    // STEP ファイルの存在を確認
    if (!QFile::exists(filePath))
    {
        QMessageBox::warning(this, "Error", QString("STEP file not found: %1").arg(filePath));
        return;
    }

    // OCAF ドキュメントを作成
    Handle(TDocStd_Document) doc = new TDocStd_Document("XmlOcaf");

    // ShapeTool と ColorTool を取得
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    // STEP ファイルを読み込む
    STEPCAFControl_Reader reader;
    reader.SetColorMode(true);
    reader.SetNameMode(true);

    IFSelect_ReturnStatus status = reader.ReadFile(filePath);
    if (status != IFSelect_RetDone)
    {
        QMessageBox::warning(this, "Error", QString("Failed to read STEP file: %1").arg(filePath));
        return;
    }

    // OCAF ドキュメントに形状を転送
    if (!reader.Transfer(doc))
    {
        QMessageBox::warning(this, "Error", QString("Failed to transfer STEP file to OCAF document."));
        return;
    }

    // ルート形状を取得
    TDF_LabelSequence freeShapes;
    shapeTool->GetFreeShapes(freeShapes);

    // ツリー構造を出力
    std::cout << "STEP File Tree Structure:" << std::endl;
    for (Standard_Integer i = 1; i <= freeShapes.Length(); i++)
    {
        TDF_Label rootLabel = freeShapes.Value(i);
        printLabelTree(rootLabel, shapeTool, colorTool, 0);
    }

    // 形状を表示
    for (Standard_Integer i = 1; i <= freeShapes.Length(); i++)
    {
        TDF_Label rootLabel = freeShapes.Value(i);
        displayShapeWithColors(rootLabel, shapeTool, colorTool);
    }

    m_view->FitAll();
}

void ViewerWidget::displayShapeWithColors(const TDF_Label &label, const Handle(XCAFDoc_ShapeTool) & shapeTool, const Handle(XCAFDoc_ColorTool) & colorTool)
{
    // ラベルに関連付けられた形状を取得
    if (shapeTool->IsShape(label))
    {
        TopoDS_Shape shape = shapeTool->GetShape(label);
        if (!shape.IsNull())
        {
            // 色を取得
            Quantity_Color color;
            bool hasColor = colorTool->GetColor(label, XCAFDoc_ColorSurf, color);

            // AIS_Shape を作成して色を設定
            Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
            if (hasColor)
            {
                aisShape->SetColor(color);
            }

            // 形状を表示
            m_context->Display(aisShape, Standard_True);
        }
    }

    // 子ラベルを再帰的に処理
    TDF_LabelSequence children;
    shapeTool->GetSubShapes(label, children);
    for (Standard_Integer i = 1; i <= children.Length(); i++)
    {
        displayShapeWithColors(children.Value(i), shapeTool, colorTool);
    }
}

void ViewerWidget::printLabelTree(const TDF_Label &label, const Handle(XCAFDoc_ShapeTool) & shapeTool, const Handle(XCAFDoc_ColorTool) & colorTool, int depth)
{
    // インデントを設定
    std::cout << std::setw(depth * 2) << "" << "- ";

    // ラベルの名前を取得
    std::ostringstream entryStream;
    label.EntryDump(entryStream);   // EntryDump に std::ostringstream を渡す
    std::cout << entryStream.str(); // ストリームの内容を文字列として出力

    // 色情報を取得
    Quantity_Color color;
    if (colorTool->GetColor(label, XCAFDoc_ColorSurf, color))
    {
        std::cout << " [Color: R=" << color.Red() << ", G=" << color.Green() << ", B=" << color.Blue() << "]";
    }

    std::cout << std::endl;

    // 子ラベルを再帰的に出力
    TDF_LabelSequence children;
    shapeTool->GetSubShapes(label, children);
    for (Standard_Integer i = 1; i <= children.Length(); i++)
    {
        printLabelTree(children.Value(i), shapeTool, colorTool, depth + 1);
    }
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