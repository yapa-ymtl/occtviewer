#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "myglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MyGLWidget* glWidget;
};
#endif // MAINWINDOW_H
----------
    #pragma once

#include <QOpenGLWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <TopoDS_Shape.hxx>

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void showEvent(QShowEvent* event) override;

private:
    Handle(OpenGl_GraphicDriver) m_driver;
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;
    TopoDS_Shape m_shape;
};
