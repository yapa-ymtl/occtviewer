#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->widget = new MyGLWidget(this);
    ui->widget->setMinimumSize(800,600);
    ui->setupUi(this);
    ui->widget->update();

}

MainWindow::~MainWindow()
{
    delete ui;
}
