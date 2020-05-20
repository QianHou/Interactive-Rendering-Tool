/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_RENDERING_VIEWER_H_
#define INC_RENDERING_VIEWER_H_

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QOpenGLTexture>
#include <QWheelEvent>

#define SHADER_VERTEX_OFFSET  (0)
#define SHADER_COLOR_OFFSET   (1)

class RenderingViewer : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit RenderingViewer(QWidget *parent);
  ~RenderingViewer();

  QVector3D getCameraPosition() { return camera_pos_; }

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;


 signals:
  void signalCameraPositionChange();

 public slots:
  void onChooseTextureImage();
  void onResetCameraPosition();

 private slots:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent*event);

 private:
  QOpenGLFunctions* fuc_;         // Qt封装的OpenGL函数对象
  QOpenGLShaderProgram* shader_;  // 渲染器对象

  QOpenGLBuffer* vertex_obj_;
  QOpenGLBuffer* color_obj_;
  QOpenGLBuffer* texture_index_obj_;
  QOpenGLVertexArrayObject* array_obj_;

  QOpenGLTexture* texture_;
  QElapsedTimer* timer_;

  float aspect_ratio_ = 0;

  GLfloat vertex_buf_[4*3*3];
  GLfloat color_buf_[4*3*3];
  GLfloat texture_index_buf_[4*3*3];

  QVector3D camera_pos_;
  QVector3D observe_center_;

  struct {
    struct {
      bool is_pressed   = false;
      size_t last_pos_x = 0;
      size_t last_pos_y = 0;
    }left, right, mid;
  }mouse_;

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

  const GLfloat COLOR_INIT_DATA[36] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
  };

  const GLfloat TEXTURE_INDEX_INIT_DATA[24] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  };
};

#endif  // INC_RENDERING_VIEWER_H_
