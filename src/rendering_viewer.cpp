/* Copyright [2020] <houqian & xiaotong> */
#include "inc/rendering_viewer.h"
#include <QFileDialog>
#include <iostream>

RenderingViewer::RenderingViewer(QWidget *parent) :
  camera_pos_(0.0f, 10.0f, 0.0f),
  observe_center_(0.0f, 0.0f, 0.0f),
  QOpenGLWidget(parent),
  fuc_(new QOpenGLFunctions()),
  objects_({new LightTextureModel(":/objects/garfield.obj"),
            new LightTextureModel(":/objects/dog.obj"),
            new LightTextureModel(":/objects/umbrella.obj")}),
  pointlights_({new PointLightModel(), new PointLightModel()}) {
  PointLightModel::setLightAmbient(0);
  pointlights_[0]->setLightPosition(QVector3D(1.5, 1.5, 1.5));
  pointlights_[1]->setLightPosition(QVector3D(-1.5, -1.5, 1.5));
}

RenderingViewer::~RenderingViewer() {
  delete fuc_;
}

void RenderingViewer::initializeGL() {
  fuc_->initializeOpenGLFunctions();
  fuc_->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  fuc_->glEnable(GL_DEPTH_TEST);

  for (const auto pointlight : pointlights_) {
    pointlight->init();
  }

  for (const auto object : objects_) {
    object->init();
  }

  objects_[0]->setTextureImage(QImage(":/images/garfield.jpg"));
  objects_[1]->setTextureImage(QImage(":/images/dog.jpg"));
  objects_[2]->setTextureImage(QImage(":/images/umbrella.jpg"));
}

void RenderingViewer::paintGL() {
  fuc_->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 view_matrix;
  view_matrix.perspective(45.0f, aspect_ratio_, 0.1f, 100.0f);
  view_matrix.lookAt(camera_pos_, observe_center_, QVector3D(0.0f, 0.0f, 1.0f));

  for (const auto& pointlight : pointlights_) {
    QMatrix4x4 model_matrix;
    model_matrix.translate(pointlight->getLightPosition());
    model_matrix.scale(0.005f);
    pointlight->paint(view_matrix, model_matrix);
  }

  std::vector<QMatrix4x4> object_model_matrix(objects_.size());
  object_model_matrix[0].scale(0.1);
  object_model_matrix[0].translate(0, 8, 0);
  // object_model_matrix[0].rotate(35, QVector3D(0, 0, 1));

  object_model_matrix[1].scale(0.02);
  object_model_matrix[1].translate(0, -20, 0);
  // object_model_matrix[1].rotate(90, QVector3D(0, 0, 1));

  object_model_matrix[2].scale(0.02);

  for (size_t i=0; i < objects_.size(); i++) {
    objects_[i]->paint(view_matrix, object_model_matrix[i], pointlights_);
  }

  // request for drawing update
  this->update();
}

void RenderingViewer::resizeGL(int width, int height) {
  aspect_ratio_ = width/height;
}

void RenderingViewer::onChooseTextureImage() {
  QString file_path = QFileDialog::getOpenFileName(this, "Please choose an image file");
  if (file_path != NULL) {
    objects_[0]->setTextureImage(QImage(file_path));
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
  camera_pos_ = QVector3D(0.0f, 10.0f, 0.0f);
  emit signalCameraPositionChange();
}