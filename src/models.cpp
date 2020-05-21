/* Copyright [2020] <houqian & xiaotong> */
#include "inc/models.h"
#include <iostream>

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

  shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/vertex.shader");
  shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/fragment.shader");
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