/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_MODULES_H_
#define INC_MODULES_H_

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#define SHADER_VERTEX_OFFSET  (0)
#define SHADER_COLOR_OFFSET   (1)

class TetrahedronModel {
 public:
  TetrahedronModel();
  ~TetrahedronModel();

  void init();
  void bind();
  void setTexture(QImage image);

 private:
  QOpenGLFunctions* fuc_;

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

  const GLfloat TEXTURE_INDEX_INIT_DATA[24] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  };
};

#endif  // INC_MODULES_H_
