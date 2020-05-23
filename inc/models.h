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
  explicit PurityModel(QString obj_file_path);
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

class PointLightModel : public PurityModel {
 public:
  PointLightModel();

  void setLightPosition(const QVector3D& position) { light_position_ = position; }
  void setLightColor(const QVector3D& color) { light_color_ = color; }
  void setLightItensity(double itensity) { light_intensity_ = itensity; }
  static void setLightAmbient(double ambient);

  QVector3D getLightPosition() const { return light_position_; }
  QVector3D getLightColor() const { return light_color_; }
  double getLightIntensity() const { return light_intensity_; }
  static double getLightAmbient();

 private:
  QVector3D light_position_;
  QVector3D  light_color_;

  double  light_intensity_;
  static double light_ambient_;
};

class TextureModel : public PurityModel {
 public:
  TextureModel();
  explicit TextureModel(QString obj_file_path);
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
  explicit LightTextureModel(QString obj_file_path);
  ~LightTextureModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix, const std::array<PointLightModel*, 2>& pointlights);

 protected:
  virtual QString getVertexShaderPath()   { return ":shaders/tripoints_light.vs"; }
  virtual QString getFragmentShaderPath() { return ":shaders/tripoints_light.fs"; }

 private:
  QOpenGLBuffer* normal_vertex_obj_;
};



#endif  // INC_MODELS_H_
