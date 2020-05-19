/* Copyright [2020] <houqian & xiaotong> */
#include "inc/rendering_viewer.h"
#include <iostream>

RenderingViewer::RenderingViewer(QWidget *parent) :
  QOpenGLWidget(parent) {}

void RenderingViewer::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
}

void RenderingViewer::paintGL() {
  glDrawArrays(GL_POINTS, 0, 1);
}

void RenderingViewer::resizeGL(int width, int height) {

}