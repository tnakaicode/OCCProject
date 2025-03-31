#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <Aspect_Handle.hxx>

class ViewerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *parent = nullptr);
    ~ViewerWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_context;
    Handle(Aspect_Window) m_window;

    void initializeOpenCASCADE();
};

#endif // VIEWERWIDGET_H