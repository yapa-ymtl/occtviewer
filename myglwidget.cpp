#include "myglwidget.h"

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>

#ifdef Q_OS_LINUX
#include <Xw_Window.hxx>
#include <X11/Xlib.h>
#endif

MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    qDebug() << "New opengl widget is created;";
}

MyGLWidget::~MyGLWidget() = default;

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

    // Do NOT create Xw_Window here!
    // Instead, do it in showEvent()

    m_shape = BRepPrimAPI_MakeBox(100.0, 50.0, 90.0).Shape();
    Handle(AIS_Shape) aisShape = new AIS_Shape(m_shape);
    m_context->Display(aisShape, true);
}

void MyGLWidget::showEvent(QShowEvent* event)
{
    qDebug() << __FUNCTION__ << " is called.";
    QOpenGLWidget::showEvent(event);

#ifdef Q_OS_LINUX
    if (!m_view.IsNull() && m_view->Window().IsNull()) {
        // Get X11 display and window id
        Display* xDisplay = static_cast<Display*>(QX11Info::display());
        Window xWindow = static_cast<Window>(winId());
        Handle(Xw_Window) occtWindow = new Xw_Window(xDisplay, xWindow);
        if (!occtWindow->IsMapped())
            occtWindow->Map();
        m_view->SetWindow(occtWindow);
        m_view->MustBeResized();
        m_view->FitAll();
    }
#endif
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