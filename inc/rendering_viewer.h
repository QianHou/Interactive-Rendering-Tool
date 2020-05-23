/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_RENDERING_VIEWER_H_
#define INC_RENDERING_VIEWER_H_

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include "inc/models.h"

#define SHADER_VERTEX_OFFSET  (0)
#define SHADER_COLOR_OFFSET   (1)

class RenderingViewer : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit RenderingViewer(QWidget *parent);
  ~RenderingViewer();

  QVector3D getCameraPosition() { return camera_pos_; }

  void setLightPosition(double x, double y, double z, int light_index) {
    if (light_index<0 || light_index>1) return;
    pointlights_[light_index]->setLightPosition(QVector3D(x, y, z));
  }
  void setLightColor(double r, double g, double b, int light_index) {
    if (light_index<0 || light_index>1) return;
    pointlights_[light_index]->setLightColor(QVector3D(r, g, b));
  }
  void setLightIntensity(double intensity, int light_index) {
    if (light_index<0 || light_index>1) return;
    pointlights_[light_index]->setLightItensity(intensity);
  }
  void setLightAmbient(double ambient) {
    PointLightModel::setLightAmbient(ambient);
  }

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

  QVector3D camera_pos_;
  QVector3D observe_center_;

  std::array<LightTextureModel*, 3> objects_;
  std::array<PointLightModel*, 2> pointlights_;

  double aspect_ratio_ = 0;

  struct {
    struct {
      bool is_pressed   = false;
      size_t last_pos_x = 0;
      size_t last_pos_y = 0;
    }left, right, mid;
  }mouse_;
};

#endif  // INC_RENDERING_VIEWER_H_
