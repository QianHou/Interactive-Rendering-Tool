/* Copyright [2020] <houqian & xiaotong> */
#include "inc/models.h"
#include <iostream>

/*********************************************四面体模型*********************************************/
TetrahedronModel::TetrahedronModel() :
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  vertex_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  texture_index_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)) {
  memcpy(this->vertex_buf_, VERTEX_INIT_DATA, sizeof(this->vertex_buf_));
  memcpy(this->texture_index_buf_, TEXTURE_INDEX_INIT_DATA, sizeof(this->texture_index_buf_));
}

TetrahedronModel::~TetrahedronModel() {
  delete fuc_;
  delete shader_;
  delete vertex_obj_;
  delete texture_index_obj_;
  delete texture_;
}

void TetrahedronModel::init() {
  fuc_->initializeOpenGLFunctions();

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/tripoints.vs");
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/tripoints.fs");
  if (!shader_->link()) {
    std::cout << "[ERROR] shaders link failed" << std::endl;
  }

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(vertex_buf_, 4*3*3* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  texture_index_obj_->create();
  texture_index_obj_->bind();
  texture_index_obj_->allocate(texture_index_buf_, 4*3*2* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
  texture_index_obj_->release();

  texture_ = new QOpenGLTexture(QImage(":images/default.jpeg"));
}

void TetrahedronModel::paint(QMatrix4x4 mvp_matrix) {
  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, 4*3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), mvp_matrix);
  shader_->release();
}

void TetrahedronModel::setTexture(QImage image) {
  delete texture_;
  texture_ = new QOpenGLTexture(image);
}

/*********************************************四面体光照模型*********************************************/
TetrahedronLightModel::TetrahedronLightModel() :
  normal_vertex_obj_(new QOpenGLBuffer()),
  light_ambient_(QVector3D(0, 0, 0)),
  light_position_({QVector3D(0.5, 0.5, 0.5), QVector3D(-0.5, -0.5, -0.5)}),
  light_color_({QVector3D(1.0, 1.0, 1.0), QVector3D(1.0, 1.0, 1.0)}),
  light_intensity_({QVector3D(0.1, 0.1, 0.1), QVector3D(0.1, 0.1, 0.1)}) {
  // 法向量求解
  for (size_t i =0 ; i < 4 ; i++) {
    std::array<GLfloat, 3> point1, point2, point3;
    memcpy(&point1, this->VERTEX_INIT_DATA + i*9+0, sizeof(point1));
    memcpy(&point2, this->VERTEX_INIT_DATA + i*9+3, sizeof(point2));
    memcpy(&point3, this->VERTEX_INIT_DATA + i*9+6, sizeof(point3));

    std::array<GLfloat, 3> vector1, vector2;
    for (size_t j = 0 ; j < 3 ; j++) {
      vector1[j] = point3[j] - point1[j];
      vector2[j] = point1[j] - point2[j];
    }

    GLfloat normal_vertex[3] = {0};
    normal_vertex[0] = vector1[1] * vector2[2] - vector2[1] * vector1[2];
    normal_vertex[1] = vector1[2] * vector2[0] - vector2[2] * vector1[0];
    normal_vertex[2] = vector1[0] * vector2[1] - vector2[0] * vector1[1];

    memcpy(normal_vertex_buf_+ i*9+0, normal_vertex, sizeof(normal_vertex));
    memcpy(normal_vertex_buf_+ i*9+3, normal_vertex, sizeof(normal_vertex));
    memcpy(normal_vertex_buf_+ i*9+6, normal_vertex, sizeof(normal_vertex));
  }
}

void TetrahedronLightModel::init() {
  fuc_->initializeOpenGLFunctions();

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/tripoints_light.vs");
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/tripoints_light.fs");
  if (!shader_->link()) {
    std::cout << "[ERROR] shaders link failed" << std::endl;
  }

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(vertex_buf_, 4*3*3* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  texture_index_obj_->create();
  texture_index_obj_->bind();
  texture_index_obj_->allocate(texture_index_buf_, 4*3*2* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
  texture_index_obj_->release();

  normal_vertex_obj_->create();
  normal_vertex_obj_->bind();
  normal_vertex_obj_->allocate(normal_vertex_buf_, 4*3*3* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_LIGHT_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_LIGHT_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  normal_vertex_obj_->release();

  texture_ = new QOpenGLTexture(QImage(":images/default.jpeg"));
}

void TetrahedronLightModel::paint(QMatrix4x4 view_matrix, QMatrix4x4 model_matrix) {
  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, 4*3);

  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), view_matrix);
  shader_->setUniformValue(shader_->uniformLocation("modelMatrix"), model_matrix);

  shader_->setUniformValue(shader_->uniformLocation("lightAmbient"), light_ambient_);

  shader_->setUniformValue(shader_->uniformLocation("lightPosition1"), light_position_[0]);
  shader_->setUniformValue(shader_->uniformLocation("lightColor1"), light_color_[0]);
  shader_->setUniformValue(shader_->uniformLocation("lightIntensity1"), light_intensity_[0]);

  shader_->setUniformValue(shader_->uniformLocation("lightPosition2"), light_position_[1]);
  shader_->setUniformValue(shader_->uniformLocation("lightColor2"), light_color_[1]);
  shader_->setUniformValue(shader_->uniformLocation("lightIntensity2"), light_intensity_[1]);

  shader_->release();
}

/*********************************************点光源模型*********************************************/
PointLightModel::PointLightModel() :
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  vertex_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)) {
  memcpy(this->vertex_buf_, VERTEX_INIT_DATA, sizeof(this->vertex_buf_));
}

PointLightModel::~PointLightModel() {
  delete fuc_;
  delete shader_;
  delete vertex_obj_;
}

void PointLightModel::init() {
  fuc_->initializeOpenGLFunctions();

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/tripoints.vs");
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/pointlight.fs");
  if (!shader_->link()) {
    std::cout << "[ERROR] shaders link failed" << std::endl;
  }

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(vertex_buf_, 4*3*3* sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();
}

void PointLightModel::paint(QMatrix4x4 mvp_matrix) {
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, 4*3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), mvp_matrix);
  shader_->release();
}