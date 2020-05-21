/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_MODELS_H_
#define INC_MODELS_H_

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#define SHADER_VERTEX_OFFSET          (0)
#define SHADER_TEXTURE_INDEX_OFFSET   (1)
#define SHADER_LIGHT_OFFSET           (2)

class TetrahedronModel {
 public:
  TetrahedronModel();
  ~TetrahedronModel();

  void init();
  void paint(QMatrix4x4 mvp_matrix);
  void setTexture(QImage image);

 protected:
  QOpenGLFunctions* fuc_;
  QOpenGLShaderProgram* shader_;  // 渲染器对象

  QOpenGLBuffer* vertex_obj_;
  QOpenGLBuffer* texture_index_obj_;

  QOpenGLTexture* texture_;

  GLfloat vertex_buf_[4*3*3];
  GLfloat texture_index_buf_[4*3*2];

  const GLfloat VERTEX_INIT_DATA[36] = {
    // face 1
    -0.5f, 0.0f, -0.2887f,
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    // face 2
    0.5f, 0.0f, -0.2887f,
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    // face 3
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    0.0f, 0.8165f, 0.0f,
    // face 4
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    0.0f, 0.0f, 0.5774f,
  };

  const GLfloat TEXTURE_INDEX_INIT_DATA[24] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  };
};

class TetrahedronLightModel : public TetrahedronModel {
 public:
  TetrahedronLightModel();

  void init();
  void paint(QMatrix4x4 view_matrix, QMatrix4x4 model_matrix);

  void setLightPosition(QVector3D position, int light_index) {
    if (light_index<0 || light_index>1) return;
    light_position_[light_index] = position;
  }
  void setLightColor(QVector3D color, int light_index) {
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

 private:
  QOpenGLBuffer* normal_vertex_obj_;
  GLfloat normal_vertex_buf_[36];

  std::array<QVector3D, 2> light_position_;
  std::array<QVector3D, 2>  light_color_;
  std::array<QVector3D, 2>  light_intensity_;

  QVector3D light_ambient_;
};

class PointLightModel {
 public:
  PointLightModel();
  ~PointLightModel();

  void init();
  void paint(QMatrix4x4 mvp_matrix);

 private:
  QOpenGLFunctions* fuc_;
  QOpenGLShaderProgram* shader_;  // 渲染器对象

  QOpenGLBuffer* vertex_obj_;

  GLfloat vertex_buf_[4*3*3];

  const GLfloat VERTEX_INIT_DATA[36] = {
    // face 1
    -0.5f, 0.0f, -0.2887f,
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    // face 2
    -0.5f, 0.0f, -0.2887f,
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    // face 3
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    0.0f, 0.8165f, 0.0f,
    // face 4
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    0.0f, 0.8165f, 0.0f,
  };
};

#endif  // INC_MODELS_H_
