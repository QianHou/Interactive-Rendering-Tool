/* Copyright [2020] <houqian & xiaotong> */
#include "inc/rendering_viewer.h"
#include <QFileDialog>
#include <iostream>

RenderingViewer::RenderingViewer(QWidget *parent) :
  QOpenGLWidget(parent),
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  array_obj_(new QOpenGLVertexArrayObject()),
  tetrahedron_(new TetrahedronModel()),
  camera_pos_(0.0f, 3.0f, 0.0f),
  observe_center_(0.0f, 0.0f, 0.0f) {
}

RenderingViewer::~RenderingViewer() {
  delete fuc_;
  delete shader_;
  delete array_obj_;
}

void RenderingViewer::initializeGL() {
  fuc_->initializeOpenGLFunctions();
  fuc_->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  fuc_->glEnable(GL_DEPTH_TEST);

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/vertex.shader");
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/fragment.shader");
  if (!shader_->link()) {
    std::cout << "[ERROR] shaders link failed" << std::endl;
  }

  array_obj_->create();
  array_obj_->bind();

  tetrahedron_->init();

  array_obj_->release();

}

void RenderingViewer::paintGL() {
  fuc_->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  array_obj_->bind();
  tetrahedron_->bind();
  shader_->bind();

  QMatrix4x4 mvp_matrix;
  mvp_matrix.perspective(45.0f, aspect_ratio_, 0.1f, 100.0f);
  mvp_matrix.lookAt(camera_pos_, observe_center_, QVector3D(0.0f, 0.0f, 1.0f));
  fuc_->glDrawArrays(GL_TRIANGLES, 0, 4*3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), mvp_matrix);

  shader_->release();
  array_obj_->release();

  // request for drawing update
  this->update();
}

void RenderingViewer::resizeGL(int width, int height) {
  aspect_ratio_ = width/height;
}

void RenderingViewer::onChooseTextureImage() {
  QString file_path = QFileDialog::getOpenFileName(this, "Please choose an image file");
  if (file_path != NULL) {
    tetrahedron_->setTexture(QImage(file_path));
  }
}

void RenderingViewer::mousePressEvent(QMouseEvent* event) {
  switch (event->button()) {
    case Qt::LeftButton:
      mouse_.left.is_pressed = true;
      mouse_.left.last_pos_x = event->x();
      mouse_.left.last_pos_y = event->y();
      break;

    case Qt::RightButton:
      mouse_.right.is_pressed = true;
      mouse_.right.last_pos_x = event->x();
      mouse_.right.last_pos_y = event->y();
      break;

  default:
    break;
  }
}

void RenderingViewer::mouseReleaseEvent(QMouseEvent* event) {
  switch (event->button()) {
    case Qt::LeftButton:
      mouse_.left.is_pressed = false;
      mouse_.left.last_pos_x = 0;
      mouse_.left.last_pos_y = 0;
      break;

    case Qt::RightButton:
      mouse_.right.is_pressed = false;
      mouse_.right.last_pos_x = 0;
      mouse_.right.last_pos_y = 0;
      break;

  default:
    break;
  }
}

void RenderingViewer::mouseMoveEvent(QMouseEvent* event) {
  if (mouse_.left.is_pressed) {
    float theta = -static_cast<int>(event->x() - mouse_.left.last_pos_x) * 0.01 + std::atan2(camera_pos_.y(), camera_pos_.x());
    float radius = sqrt(pow(camera_pos_.x(), 2)+ pow(camera_pos_.y(), 2));
    camera_pos_.setX(radius * std::cos(theta));
    camera_pos_.setY(radius * std::sin(theta));

    camera_pos_.setZ(camera_pos_.z() + static_cast<int>(event->y() - mouse_.left.last_pos_y) * 0.01);

    mouse_.left.last_pos_x = event->x();
    mouse_.left.last_pos_y = event->y();
  }
  emit signalCameraPositionChange();
}

void RenderingViewer::wheelEvent(QWheelEvent*event) {
  camera_pos_.setX(camera_pos_.x() * (1 - event->delta()*0.001));
  camera_pos_.setY(camera_pos_.y() * (1 - event->delta()*0.001));
  camera_pos_.setZ(camera_pos_.z() * (1 - event->delta()*0.001));
  emit signalCameraPositionChange();
}

void RenderingViewer::onResetCameraPosition() {
  camera_pos_ = QVector3D(0.0f, 3.0f, 0.0f);
  emit signalCameraPositionChange();
}