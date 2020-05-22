/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_MODELS_H_
#define INC_MODELS_H_

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <vector>
#include "inc/object_loader.h"

#define SHADER_VERTEX_OFFSET          (0)
#define SHADER_TEXTURE_INDEX_OFFSET   (1)
#define SHADER_LIGHT_OFFSET           (2)

class PurityModel : public ObjectLoader {
 public:
  PurityModel();
  ~PurityModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix);

 protected:
  virtual QString getVertexShaderPath()   { return ":shaders/tripoints.vs"; }
  virtual QString getFragmentShaderPath() { return ":shaders/purity.fs"; }

 protected:
  QOpenGLFunctions* fuc_;
  QOpenGLShaderProgram* shader_;  // 渲染器对象

  QOpenGLBuffer* vertex_obj_;
  QOpenGLVertexArrayObject* array_obj_;
};

class TextureModel : public PurityModel {
 public:
  TextureModel();
  ~TextureModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix);

  void setTextureImage(QImage image);

 protected:
  virtual QString getVertexShaderPath()   { return ":shaders/tripoints.vs"; }
  virtual QString getFragmentShaderPath() { return ":shaders/tripoints.fs"; }

 protected:
  QOpenGLBuffer* texture_index_obj_;
  QOpenGLTexture* texture_ = NULL;
};

class LightTextureModel : public TextureModel {
 public:
  LightTextureModel();
  ~LightTextureModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix);

  void setLightPosition(const QVector3D& position, int light_index) {
    if (light_index<0 || light_index>1) return;
    light_position_[light_index] = position;
  }
  void setLightColor(const QVector3D& color, int light_index) {
    if (light_index<0 || light_index>1) return;
    light_color_[light_index] = color;
  }
  void setLightItensity(double itensity, int light_index) {
    if (light_index<0 || light_index>1) return;
    light_intensity_[light_index] = QVector3D(itensity, itensity, itensity);
  }
  void setLightAmbient(double ambient) {
    light_ambient_ = QVector3D(ambient, ambient, ambient);
  }

  QVector3D getLightPosition(int light_index) {
    if (light_index<0 || light_index>1) return QVector3D(0, 0, 0);
    return light_position_[light_index];
  }
  QVector3D getLightColor(int light_index) {
    if (light_index<0 || light_index>1) return QVector3D(0, 0, 0);
    return light_color_[light_index];
  }
  double getLightIntensity(int light_index) {
    if (light_index<0 || light_index>1) return 0;
    return light_intensity_[light_index].x();
  }

 protected:
  virtual QString getVertexShaderPath()   { return ":shaders/tripoints_light.vs"; }
  virtual QString getFragmentShaderPath() { return ":shaders/tripoints_light.fs"; }

 private:
  QOpenGLBuffer* normal_vertex_obj_;

  std::array<QVector3D, 2> light_position_;
  std::array<QVector3D, 2>  light_color_;
  std::array<QVector3D, 2>  light_intensity_;

  QVector3D light_ambient_;
};

#endif  // INC_MODELS_H_
