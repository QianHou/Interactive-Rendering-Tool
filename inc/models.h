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
  PurityModel(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data);
  ~PurityModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix);
  void rebind(QString obj_file_path);

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
  TextureModel(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data);
  ~TextureModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix);
  void rebind(QString obj_file_path);

  void setTextureImage(QImage image);

 protected:
  virtual QString getVertexShaderPath()   { return ":shaders/tripoints.vs"; }
  virtual QString getFragmentShaderPath() { return ":shaders/tripoints.fs"; }

  virtual QString getTextureImagePath() { return ":images/default.jpeg"; }

 protected:
  QOpenGLBuffer* texture_index_obj_;
  QOpenGLTexture* texture_ = NULL;
};

class LightTextureModel : public TextureModel {
 public:
  LightTextureModel();
  explicit LightTextureModel(QString obj_file_path);
  LightTextureModel(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data);
  ~LightTextureModel();

  void init();
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix);
  void paint(const QMatrix4x4& view_matrix, const QMatrix4x4& model_matrix, const std::array<PointLightModel*, 2>& pointlights);

  void rebind(QString obj_file_path);

 protected:
  virtual QString getVertexShaderPath()   { return ":shaders/tripoints_light.vs"; }
  virtual QString getFragmentShaderPath() { return ":shaders/tripoints_light.fs"; }

 protected:
  QOpenGLBuffer* normal_vertex_obj_;
};

class GroundModel : public LightTextureModel {
 public:
  GroundModel();

  void warpNormalVector(QImage normal_image);
  void resetNormalVector();

 private:
  void updateNormalVector();

 protected:
  virtual QString getTextureImagePath() { return ":images/grass.jpg"; }

 private:
  std::vector<GLfloat> warped_normal_vector_;
  std::vector<GLfloat> raw_normal_vector_;

};



#endif  // INC_MODELS_H_
