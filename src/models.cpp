/* Copyright [2020] <houqian & xiaotong> */
#include "inc/models.h"
#include <QColor>
#include <iostream>

/*********************************************纯色模型*********************************************/
PurityModel::PurityModel():
  ObjectLoader(ObjectData::TETRAHEDRO_VERTEX, ObjectData::TETRAHEDRO_TEXTURE_INDEX),
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  vertex_obj_(new QOpenGLBuffer()),
  array_obj_(new QOpenGLVertexArrayObject()) {}

PurityModel::PurityModel(QString obj_file_path):
  ObjectLoader(obj_file_path),
  fuc_(new QOpenGLFunctions()),
  shader_(new QOpenGLShaderProgram()),
  vertex_obj_(new QOpenGLBuffer()),
  array_obj_(new QOpenGLVertexArrayObject()) {}

PurityModel::PurityModel(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data):
  ObjectLoader(vertex_data, texture_index_data),
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

  if (this->vertex_.buffer) {
    vertex_obj_->create();
    vertex_obj_->bind();
    vertex_obj_->allocate(this->vertex_.buffer, this->vertex_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    vertex_obj_->release();
  } else {
    std::cout << "[ERROR] vertex buffer not create" << std::endl;
  }

  array_obj_->release();
}

void PurityModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix) {
  array_obj_->bind();

  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, this->vertex_.size/3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), view_matrix);
  shader_->setUniformValue(shader_->uniformLocation("modelMatrix"), model_matrix);
  shader_->release();

  array_obj_->release();
}

void PurityModel::rebind(QString obj_file_path) {
  this->reloadObject(obj_file_path);

  array_obj_->bind();
  if (this->vertex_.buffer) {
    vertex_obj_->bind();
    vertex_obj_->allocate(this->vertex_.buffer, this->vertex_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_VERTEX_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_VERTEX_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    vertex_obj_->release();
  } else {
    std::cout << "[ERROR] vertex buffer not create" << std::endl;
  }
  array_obj_->release();
}

/*********************************************点光源模型*********************************************/
PointLightModel::PointLightModel() :
  PurityModel(":/objects/ball.obj"),
  light_position_(QVector3D(0.5, 0.5, 0.5)),
  light_color_(QVector3D(1.0, 1.0, 1.0)),
  light_intensity_(0.5) {}

double PointLightModel::light_ambient_ = 0;

void PointLightModel::setLightAmbient(double ambient) {
  light_ambient_ = ambient;
}

double PointLightModel::getLightAmbient() {
  return light_ambient_;
}

/*********************************************纹理映射模型*********************************************/
TextureModel::TextureModel() :
  texture_index_obj_(new QOpenGLBuffer()) {}

TextureModel::TextureModel(QString obj_file_path):
  PurityModel(obj_file_path),
  texture_index_obj_(new QOpenGLBuffer()) {}

TextureModel::TextureModel(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data):
  PurityModel(vertex_data, texture_index_data),
  texture_index_obj_(new QOpenGLBuffer()) {}

TextureModel::~TextureModel() {
  if (texture_) delete texture_;
  delete texture_index_obj_;
}

void TextureModel::init() {
  PurityModel::init();

  texture_ = new QOpenGLTexture(QImage(getTextureImagePath()));

  array_obj_->bind();

  if (this->texture_index_.buffer) {
    texture_index_obj_->create();
    texture_index_obj_->bind();
    texture_index_obj_->allocate(this->texture_index_.buffer, this->texture_index_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
    texture_index_obj_->release();
  } else {
    std::cout << "[ERROR] texture index buffer not create" << std::endl;
  }

  array_obj_->release();
}

void TextureModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix) {
  array_obj_->bind();

  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, this->vertex_.size/3);
  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), view_matrix);
  shader_->setUniformValue(shader_->uniformLocation("modelMatrix"), model_matrix);
  shader_->release();

  array_obj_->release();
}

void TextureModel::rebind(QString obj_file_path) {
  PurityModel::rebind(obj_file_path);

  array_obj_->bind();
  if (this->texture_index_.buffer) {
    texture_index_obj_->bind();
    texture_index_obj_->allocate(this->texture_index_.buffer, this->texture_index_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_TEXTURE_INDEX_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_TEXTURE_INDEX_OFFSET, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
    texture_index_obj_->release();
  } else {
    std::cout << "[ERROR] texture index buffer not create" << std::endl;
  }
  array_obj_->release();
}

void TextureModel::setTextureImage(QImage image) {
  if (texture_) delete texture_;
  texture_ = new QOpenGLTexture(image);
}

/*********************************************光照纹理映射模型*********************************************/
LightTextureModel::LightTextureModel() :
  normal_vertex_obj_(new QOpenGLBuffer()) {}

LightTextureModel::LightTextureModel(QString obj_file_path) :
  TextureModel(obj_file_path),
  normal_vertex_obj_(new QOpenGLBuffer()) {}

LightTextureModel::LightTextureModel(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data) :
  TextureModel(vertex_data, texture_index_data),
  normal_vertex_obj_(new QOpenGLBuffer()) {}

LightTextureModel::~LightTextureModel() {
  delete normal_vertex_obj_;
}

void LightTextureModel::init() {
  TextureModel::init();

  array_obj_->bind();

  if (this->normal_vector_.buffer) {
    normal_vertex_obj_->create();
    normal_vertex_obj_->bind();
    normal_vertex_obj_->allocate(this->normal_vector_.buffer, this->normal_vector_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_LIGHT_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_LIGHT_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    normal_vertex_obj_->release();
  } else {
    std::cout << "[ERROR] normal vector buffer not create" << std::endl;
  }

  array_obj_->release();
}

void LightTextureModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix) {
  std::cout << "[WARN] no point light in LightTextureModel" << std::endl;
  TextureModel::paint(view_matrix, model_matrix);
}

void LightTextureModel::paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix, const std::array<PointLightModel*, 2>& pointlights) {
  array_obj_->bind();

  texture_->bind();
  shader_->bind();
  fuc_->glDrawArrays(GL_TRIANGLES, 0, this->vertex_.size/3);

  shader_->setUniformValue(shader_->uniformLocation("viewMatrix"), view_matrix);
  shader_->setUniformValue(shader_->uniformLocation("modelMatrix"), model_matrix);

  double ambient = PointLightModel::getLightAmbient();
  QVector3D ambient_vector = QVector3D(ambient, ambient, ambient);
  shader_->setUniformValue(shader_->uniformLocation("lightAmbient"), ambient_vector);

  shader_->setUniformValue(shader_->uniformLocation("lightPosition1"), pointlights[0]->getLightPosition());
  shader_->setUniformValue(shader_->uniformLocation("lightColor1"), pointlights[0]->getLightColor());

  double intensity1 = pointlights[0]->getLightIntensity();
  QVector3D intensity1_vector = QVector3D(intensity1, intensity1, intensity1);
  shader_->setUniformValue(shader_->uniformLocation("lightIntensity1"), intensity1_vector);

  shader_->setUniformValue(shader_->uniformLocation("lightPosition2"), pointlights[1]->getLightPosition());
  shader_->setUniformValue(shader_->uniformLocation("lightColor2"), pointlights[1]->getLightColor());

  double intensity2 = pointlights[1]->getLightIntensity();
  QVector3D intensity2_vector = QVector3D(intensity2, intensity2, intensity2);
  shader_->setUniformValue(shader_->uniformLocation("lightIntensity2"), intensity2_vector);

  shader_->release();

  array_obj_->release();
}

void LightTextureModel::rebind(QString obj_file_path) {
  TextureModel::rebind(obj_file_path);

  array_obj_->bind();
  if (this->normal_vector_.buffer) {
    normal_vertex_obj_->bind();
    normal_vertex_obj_->allocate(this->normal_vector_.buffer, this->normal_vector_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_LIGHT_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_LIGHT_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    normal_vertex_obj_->release();
  } else {
    std::cout << "[ERROR] normal vector buffer not create" << std::endl;
  }
  array_obj_->release();
}

/*********************************************地面模型*********************************************/
GroundModel::GroundModel() :
  LightTextureModel(":/objects/square.obj") {
  // modify texture index
  for (size_t i = 0; i < this->texture_index_.size/6 ; i++) {
    std::array<GLfloat, 6> texture = {this->vertex_.buffer[i*9+0]+0.5f,
                                      this->vertex_.buffer[i*9+2]+0.5f,
                                      this->vertex_.buffer[i*9+3]+0.5f,
                                      this->vertex_.buffer[i*9+5]+0.5f,
                                      this->vertex_.buffer[i*9+6]+0.5f,
                                      this->vertex_.buffer[i*9+8]+0.5f};
    std::copy(texture.begin(), texture.end(), texture_index_.buffer + i*6);
  }
  raw_normal_vector_.resize(this->normal_vector_.size);
  std::copy(this->normal_vector_.buffer, this->normal_vector_.buffer+this->normal_vector_.size, raw_normal_vector_.begin());
}

void GroundModel::warpNormalVector(QImage normal_image) {
  if (normal_image.size() == QSize(0, 0)) {
    std::cout << "[WARN] size of normal image is zero" << std::endl;
    return;
  }
  if (this->vertex_.size/9 != this->texture_index_.size/6 || this->vertex_.size/9 != this->normal_vector_.size/9) {
    std::cout << "[ERROR] invalid data length" << std::endl;
    return;
  }

  warped_normal_vector_.clear();
  for (size_t i = 0; i < this->texture_index_.size/2 ; i++) {
    std::array<GLfloat, 3> vertex = {this->vertex_.buffer[i*3+0], this->vertex_.buffer[i*3+1], this->vertex_.buffer[i*3+2]};
    std::array<GLfloat, 2> texture = {this->texture_index_.buffer[i*2+0], this->texture_index_.buffer[i*2+1]};
    std::array<GLfloat, 2> pix_index = {texture[0] * normal_image.width(), (1-texture[1]) * normal_image.height()};

    if (pix_index[0] < 0) pix_index[0] = 0;
    if (pix_index[0] > normal_image.width()-1) pix_index[0] = normal_image.width()-1;
    if (pix_index[1] < 0) pix_index[1] = 0;
    if (pix_index[1] > normal_image.height()-1) pix_index[1] = normal_image.height()-1;

    warped_normal_vector_.push_back(QColor(normal_image.pixel(QPoint(pix_index[0], pix_index[1]))).red()   *2.0f-1.0f);
    warped_normal_vector_.push_back(QColor(normal_image.pixel(QPoint(pix_index[0], pix_index[1]))).green() *2.0f-1.0f);
    warped_normal_vector_.push_back(QColor(normal_image.pixel(QPoint(pix_index[0], pix_index[1]))).blue()  *2.0f-1.0f);
  }

  if (warped_normal_vector_.size() == this->normal_vector_.size) {
    std::copy(warped_normal_vector_.begin(), warped_normal_vector_.end(), normal_vector_.buffer);
  }
  updateNormalVector();
}

void GroundModel::resetNormalVector() {
  if (raw_normal_vector_.size() == this->normal_vector_.size) {
    std::copy(raw_normal_vector_.begin(), raw_normal_vector_.end(), normal_vector_.buffer);
  }
  updateNormalVector();
}

void GroundModel::updateNormalVector() {
  if (this->normal_vector_.buffer) {
    normal_vertex_obj_->create();
    normal_vertex_obj_->bind();
    normal_vertex_obj_->allocate(this->normal_vector_.buffer, this->normal_vector_.size * sizeof(GLfloat));
    fuc_->glEnableVertexAttribArray(SHADER_LIGHT_OFFSET);
    fuc_->glVertexAttribPointer(SHADER_LIGHT_OFFSET, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    normal_vertex_obj_->release();
  } else {
    std::cout << "[ERROR] normal vector buffer not create" << std::endl;
  }
}
