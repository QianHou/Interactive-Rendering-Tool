/* Copyright [2020] <houqian & xiaotong> */
#include "inc/models.h"
#include <iostream>

/*********************************************纯色模型*********************************************/
PurityModel::PurityModel():
  ObjectLoader(ObjectData::TETRAHEDRO_VERTEX, ObjectData::TETRAHEDRO_TEXTURE_INDEX),
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  vertex_obj_(new QOpenGLBuffer()),
  array_obj_(new QOpenGLVertexArrayObject()) {}

PurityModel::~PurityModel() {
  delete fuc_;
  delete shader_;
  delete vertex_obj_;
  delete array_obj_;
}

void PurityModel::init() {
  fuc_->initializeOpenGLFunctions();

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, getVertexShaderPath());
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, getFragmentShaderPath());
  if (!shader_->link()) {
    std::cout << "[ERROR] shaders link failed" << std::endl;
  }

  array_obj_->create();
  array_obj_->bind();

  vertex_obj_->create();
  vertex_obj_->bind();
  vertex_obj_->allocate(this->vertex_.buffer, this->vertex_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  vertex_obj_->release();

  array_obj_->release();
}

void PurityModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix) {
  array_obj_->bind();

  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, this->vertex_.size/3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), view_matrix*model_matrix);
  shader_->release();

  array_obj_->release();
}

/*********************************************纹理映射模型*********************************************/
TextureModel::TextureModel() :
  texture_index_obj_(new QOpenGLBuffer()) {}

TextureModel::~TextureModel() {
  if (texture_) delete texture_;
  delete texture_index_obj_;
}

void TextureModel::init() {
  PurityModel::init();
  texture_ = new QOpenGLTexture(QImage(":images/default.jpeg"));

  array_obj_->bind();

  texture_index_obj_->create();
  texture_index_obj_->bind();
  texture_index_obj_->allocate(this->texture_index_.buffer, this->texture_index_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
  texture_index_obj_->release();

  array_obj_->release();
}

void TextureModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix) {
  PurityModel::paint(view_matrix, model_matrix);

  array_obj_->bind();
  texture_->bind();
  array_obj_->release();
}

void TextureModel::setTextureImage(QImage image) {
  if (texture_) delete texture_;
  texture_ = new QOpenGLTexture(image);
}

/*********************************************光照纹理映射模型*********************************************/
LightTextureModel::LightTextureModel() :
  normal_vertex_obj_(new QOpenGLBuffer()),
  light_ambient_(QVector3D(0, 0, 0)),
  light_position_({QVector3D(0.5, 0.5, 0.5), QVector3D(-0.5, -0.5, -0.5)}),
  light_color_({QVector3D(1.0, 1.0, 1.0), QVector3D(1.0, 1.0, 1.0)}),
  light_intensity_({QVector3D(0.5, 0.5, 0.5), QVector3D(0.5, 0.5, 0.5)}) {}

LightTextureModel::~LightTextureModel() {
  delete normal_vertex_obj_;
}

void LightTextureModel::init() {
  TextureModel::init();

  array_obj_->bind();

  normal_vertex_obj_->create();
  normal_vertex_obj_->bind();
  normal_vertex_obj_->allocate(this->normal_vector_.buffer, this->normal_vector_.size * sizeof(GLfloat));
  fuc_->glEnableVertexAttribArray(SHADER_LIGHT_OFFSET);
  fuc_->glVertexAttribPointer(SHADER_LIGHT_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  normal_vertex_obj_->release();

  array_obj_->release();
}

void LightTextureModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix) {
  array_obj_->bind();

  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, this->vertex_.size/3);

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
