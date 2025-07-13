#include "myglwidget.h"


#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
//#include <WNT_Window.hxx> // Optional on Windows, doesn't work on Mac/Linux
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>

#ifdef __APPLE__
#include <Cocoa_Window.hxx>
#endif


MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    qDebug() << "New opengl widget is created;";
}

MyGLWidget::~MyGLWidget() = default;

#include <Cocoa_Window.hxx>  // Make sure this is included

void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions();

    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();

    if (m_driver.IsNull()) {
        m_driver = new OpenGl_GraphicDriver(displayConnection);
    }

    Handle(V3d_Viewer) viewer = new V3d_Viewer(m_driver);
    viewer->SetDefaultLights();
    viewer->SetLightOn();

    m_context = new AIS_InteractiveContext(viewer);
    m_view = viewer->CreateView();

    m_view->SetBackgroundColor(Quantity_NOC_BLACK);

    // Do NOT create Cocoa_Window here!
    // Instead, do it in showEvent()

    m_shape = BRepPrimAPI_MakeBox(100.0, 50.0, 90.0).Shape();
    Handle(AIS_Shape) aisShape = new AIS_Shape(m_shape);
    m_context->Display(aisShape, true);
}


void MyGLWidget::showEvent(QShowEvent* event)
{
    qDebug() << __FUNCTION__ << " is called.";
    QOpenGLWidget::showEvent(event);

    if (!m_view.IsNull() && m_view->Window().IsNull()) {
#ifdef __APPLE__
        NSView* cocoaView = reinterpret_cast<NSView*>(this->winId());
        Handle(Cocoa_Window) occtWindow = new Cocoa_Window(cocoaView);
        if (!occtWindow->IsMapped())
            occtWindow->Map();
        m_view->SetWindow(occtWindow);
#endif
        m_view->MustBeResized();
        m_view->FitAll();
    }
    if (!m_view.IsNull())
        m_view->Redraw();
}

void MyGLWidget::resizeGL(int w, int h)
{
    if (!m_view.IsNull() && !m_view->Window().IsNull())
    {
        qDebug() << __FUNCTION__ << " is called.";
        m_view->Window()->DoResize();
        m_view->MustBeResized();
        m_view->FitAll();
    }
}


void MyGLWidget::paintGL()
{
    if (!m_view.IsNull())
        m_view->Redraw();
}
