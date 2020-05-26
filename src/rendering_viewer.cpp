/* Copyright [2020] <houqian & xiaotong> */
#include "inc/rendering_viewer.h"
#include <QFileDialog>
#include <iostream>

RenderingViewer::RenderingViewer(QWidget *parent) :
  observe_center_(0.0f, 0.0f, 0.0f),
  camera_pos_(GlobalParams::CAMERA_POSITION_INIT),
  QOpenGLWidget(parent),
  fuc_(new QOpenGLFunctions()),
  objects_({new LightTextureModel(GlobalParams::OBJECT1_OBJ_PATH_INIT),
            new LightTextureModel(GlobalParams::OBJECT2_OBJ_PATH_INIT),
            new LightTextureModel(GlobalParams::OBJECT3_OBJ_PATH_INIT)}),
  pointlights_({new PointLightModel(), new PointLightModel()}) {
  objects_pose_[0].position = GlobalParams::OBJECT1_POSITION_INIT;
  objects_pose_[0].rotate = GlobalParams::OBJECT1_ROTATE_INIT;
  objects_pose_[0].scale = GlobalParams::OBJECT1_SCALE_INIT;
  objects_name_[0] = GlobalParams::OBJECT1_OBJ_PATH_INIT.split('/').last();

  objects_pose_[1].position = GlobalParams::OBJECT2_POSITION_INIT;
  objects_pose_[1].rotate = GlobalParams::OBJECT2_ROTATE_INIT;
  objects_pose_[1].scale = GlobalParams::OBJECT2_SCALE_INIT;
  objects_name_[1] = GlobalParams::OBJECT2_OBJ_PATH_INIT.split('/').last();

  objects_pose_[2].position = GlobalParams::OBJECT3_POSITION_INIT;
  objects_pose_[2].rotate = GlobalParams::OBJECT3_ROTATE_INIT;
  objects_pose_[2].scale = GlobalParams::OBJECT3_SCALE_INIT;
  objects_name_[2] = GlobalParams::OBJECT3_OBJ_PATH_INIT.split('/').last();

  PointLightModel::setLightAmbient(GlobalParams::AMBIENT_LIGHT_INIT);
  pointlights_[0]->setLightPosition(QVector3D(GlobalParams::POINTLIGHT1_POSITION_INIT));
  pointlights_[0]->setLightItensity(GlobalParams::POINTLIGHT1_ITENSITY_INIT);
  pointlights_[1]->setLightPosition(QVector3D(GlobalParams::POINTLIGHT2_POSITION_INIT));
  pointlights_[1]->setLightItensity(GlobalParams::POINTLIGHT2_ITENSITY_INIT);
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

  objects_[0]->setTextureImage(QImage(GlobalParams::OBJECT1_TEXTURE_INIT));
  objects_[1]->setTextureImage(QImage(GlobalParams::OBJECT2_TEXTURE_INIT));
  objects_[2]->setTextureImage(QImage(GlobalParams::OBJECT3_TEXTURE_INIT));
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

  for (size_t i=0; i < objects_.size(); i++) {
    QMatrix4x4 model_matrix;
    model_matrix.scale(objects_pose_[i].scale);
    model_matrix.translate(objects_pose_[i].position);
    model_matrix.rotate(objects_pose_[i].rotate.x(), QVector3D(1, 0, 0));
    model_matrix.rotate(objects_pose_[i].rotate.y(), QVector3D(0, 1, 0));
    model_matrix.rotate(objects_pose_[i].rotate.z(), QVector3D(0, 0, 1));
    objects_[i]->paint(view_matrix, model_matrix, pointlights_);
  }

  // request for drawing update
  this->update();
}

void RenderingViewer::resizeGL(int width, int height) {
  aspect_ratio_ = width/height;
}

void RenderingViewer::onChooseTextureImageOfObject1() {
  QString file_path = QFileDialog::getOpenFileName(this, "请选择图片文件", ".",
                                                   "JPEG Files(*.jpg *.jpeg);;PNG Files(*.png);;BMP Files(*.bmp);;PGM Files(*.pgm)");
  if (file_path != NULL && objects_.size() > 0) {
    objects_[0]->setTextureImage(QImage(file_path));
  }
}

void RenderingViewer::onChooseModelOfObject1() {
  QString file_path = QFileDialog::getOpenFileName(this, "请选择模型文件", ".", "OBJ Files(*.obj)");
  if (file_path != NULL && objects_.size() > 0) {
    objects_[0]->rebind(file_path);
    objects_name_[0] = file_path.split('/').last();
    emit signalObjectNameChange();
  }
}

void RenderingViewer::onChooseTextureImageOfObject2() {
  QString file_path = QFileDialog::getOpenFileName(this, "请选择图片文件", ".",
                                                   "JPEG Files(*.jpg *.jpeg);;PNG Files(*.png);;BMP Files(*.bmp);;PGM Files(*.pgm)");
  if (file_path != NULL && objects_.size() > 1) {
    objects_[1]->setTextureImage(QImage(file_path));
  }
}

void RenderingViewer::onChooseModelOfObject2() {
  QString file_path = QFileDialog::getOpenFileName(this, "请选择模型文件", ".", "OBJ Files(*.obj)");
  if (file_path != NULL && objects_.size() > 1) {
    objects_[1]->rebind(file_path);
    objects_name_[1] = file_path.split('/').last();
    emit signalObjectNameChange();
  }
}

void RenderingViewer::onChooseTextureImageOfObject3() {
  QString file_path = QFileDialog::getOpenFileName(this, "请选择图片文件", ".",
                                                   "JPEG Files(*.jpg *.jpeg);;PNG Files(*.png);;BMP Files(*.bmp);;PGM Files(*.pgm)");
  if (file_path != NULL && objects_.size() > 2) {
    objects_[2]->setTextureImage(QImage(file_path));
  }
}

void RenderingViewer::onChooseModelOfObject3() {
  QString file_path = QFileDialog::getOpenFileName(this, "请选择模型文件", ".", "OBJ Files(*.obj)");
  if (file_path != NULL && objects_.size() > 2) {
    objects_[2]->rebind(file_path);
    objects_name_[2] = file_path.split('/').last();
    emit signalObjectNameChange();
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
  camera_pos_ = QVector3D(GlobalParams::CAMERA_POSITION_INIT);
  emit signalCameraPositionChange();
}