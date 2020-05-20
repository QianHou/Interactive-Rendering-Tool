/* Copyright [2020] <houqian & xiaotong> */
#include "inc/rendering_viewer.h"
#include <QFileDialog>
#include <iostream>

RenderingViewer::RenderingViewer(QWidget *parent) :
  QOpenGLWidget(parent),
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  vertex_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  color_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  texture_index_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  array_obj_(new QOpenGLVertexArrayObject()),
  timer_(new QElapsedTimer()),
  camera_pos_(0.0f, 3.0f, 0.0f),
  observe_center_(0.0f, 0.0f, 0.0f) {
  memcpy(this->vertex_buf_, VERTEX_INIT_DATA, sizeof(this->vertex_buf_));
  memcpy(this->color_buf_, COLOR_INIT_DATA, sizeof(this->color_buf_));
  memcpy(this->texture_index_buf_, TEXTURE_INDEX_INIT_DATA, sizeof(this->texture_index_buf_));

  timer_->start();
}

RenderingViewer::~RenderingViewer() {
  delete fuc_;
  delete shader_;
  delete vertex_obj_;
  delete color_obj_;
  delete texture_index_obj_;
  delete array_obj_;
  delete texture_;
  delete timer_;
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

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(vertex_buf_, 4*3*3* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  color_obj_->create();
  color_obj_->bind();
  color_obj_->allocate(color_buf_, 4*3*3* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_COLOR_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_COLOR_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  color_obj_->release();

  texture_index_obj_->create();
  texture_index_obj_->bind();
  texture_index_obj_->allocate(texture_index_buf_, 4*3*2* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(2);
  fuc_->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
  texture_index_obj_->release();

  array_obj_->release();

  texture_ = new QOpenGLTexture(QImage(":images/default.jpeg"));
}

void RenderingViewer::paintGL() {
  fuc_->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  array_obj_->bind();
  shader_->bind();
  texture_->bind();

  QMatrix4x4 mvp_matrix;
  mvp_matrix.perspective(45.0f, aspect_ratio_, 0.1f, 100.0f);
  mvp_matrix.lookAt(camera_pos_, observe_center_, QVector3D(0.0f, 0.0f, 1.0f));
  float time_s = (float)timer_->elapsed() / 1000;
  // mvp_matrix.rotate(30.0f * time_s, QVector3D(0.0f, 0.0f, 1.0f));
  fuc_->glDrawArrays(GL_TRIANGLES, 0, 4*3);
  shader_->setUniformValue(shader_->uniformLocation("MVP"), mvp_matrix);

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
    delete texture_;
    texture_ = new QOpenGLTexture(QImage(file_path));
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
    float theta = static_cast<int>(event->x() - mouse_.left.last_pos_x) * 0.01 + std::atan2(camera_pos_.y(), camera_pos_.x());
    float radius = sqrt(pow(camera_pos_.x(), 2)+ pow(camera_pos_.y(), 2));
    camera_pos_.setX(radius * std::cos(theta));
    camera_pos_.setY(radius * std::sin(theta));

    // camera_pos_.setX(camera_pos_.x() + static_cast<int>(event->x() - mouse_.left.last_pos_x) * 0.01);
    // camera_pos_.setZ(camera_pos_.z() + static_cast<int>(event->y() - mouse_.left.last_pos_y) * 0.01);
    mouse_.left.last_pos_x = event->x();
    mouse_.left.last_pos_y = event->y();
  }
  std::cout << camera_pos_.x() << " , " << camera_pos_.y() << " , " << camera_pos_.z() << std::endl;
}

void RenderingViewer::wheelEvent(QWheelEvent*event) {
  camera_pos_.setX(camera_pos_.x() * (1 - event->delta()*0.001));
  camera_pos_.setY(camera_pos_.y() * (1 - event->delta()*0.001));
  camera_pos_.setZ(camera_pos_.z() * (1 - event->delta()*0.001));
  std::cout << camera_pos_.x() << " , " << camera_pos_.y() << " , " << camera_pos_.z() << std::endl;

}
