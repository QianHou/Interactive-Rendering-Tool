/* Copyright [2020] <houqian & xiaotong> */
#include "inc/models.h"
#include <iostream>

/*********************************************四面体模型*********************************************/
TetrahedronModel::TetrahedronModel() :
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  loader_(new ObjectLoader(":objects/penguin.obj")),
  vertex_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  texture_index_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  array_obj_(new QOpenGLVertexArrayObject()) {}

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

  texture_ = new QOpenGLTexture(QImage(":images/default.jpeg"));

  array_obj_->create();
  array_obj_->bind();

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(loader_->vertex_.buffer, loader_->vertex_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  texture_index_obj_->create();
  texture_index_obj_->bind();
  texture_index_obj_->allocate(loader_->texture_index_.buffer, loader_->texture_index_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
  texture_index_obj_->release();

  array_obj_->release();
}

void TetrahedronModel::paint(QMatrix4x4 mvp_matrix) {
  array_obj_->bind();

  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, loader_->vertex_.size/3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), mvp_matrix);
  shader_->release();

  array_obj_->release();
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
  light_intensity_({QVector3D(0.5, 0.5, 0.5), QVector3D(0.5, 0.5, 0.5)}) {}

void TetrahedronLightModel::init() {
  fuc_->initializeOpenGLFunctions();

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/tripoints_light.vs");
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/tripoints_light.fs");
  if (!shader_->link()) {
    std::cout << "[ERROR] shaders link failed" << std::endl;
  }

  texture_ = new QOpenGLTexture(QImage(":images/default.jpeg"));

  array_obj_->create();
  array_obj_->bind();

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(loader_->vertex_.buffer, loader_->vertex_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  texture_index_obj_->create();
  texture_index_obj_->bind();
  texture_index_obj_->allocate(loader_->texture_index_.buffer, loader_->texture_index_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
  texture_index_obj_->release();

  normal_vertex_obj_->create();
  normal_vertex_obj_->bind();
  normal_vertex_obj_->allocate(loader_->normal_vector_.buffer, loader_->normal_vector_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_LIGHT_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_LIGHT_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  normal_vertex_obj_->release();

  array_obj_->release();

}

void TetrahedronLightModel::paint(QMatrix4x4 view_matrix, QMatrix4x4 model_matrix) {
  array_obj_->bind();

  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, loader_->vertex_.size/3);

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

  array_obj_->release();
}

/*********************************************点光源模型*********************************************/
PointLightModel::PointLightModel() :
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  loader_(new ObjectLoader(ObjectData::TETRAHEDRO_VERTEX, ObjectData::TETRAHEDRO_TEXTURE_INDEX)),
  vertex_obj_(new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer)),
  array_obj_(new QOpenGLVertexArrayObject()) {}

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

  array_obj_->create();
  array_obj_->bind();

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(loader_->vertex_.buffer, loader_->vertex_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  array_obj_->release();
}

void PointLightModel::paint(QMatrix4x4 mvp_matrix) {
  array_obj_->bind();

  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, loader_->vertex_.size/3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), mvp_matrix);
  shader_->release();

  array_obj_->release();
}