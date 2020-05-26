/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_RENDERING_VIEWER_H_
#define INC_RENDERING_VIEWER_H_

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include "inc/models.h"
#include "inc/parameters.h"

#define SHADER_VERTEX_OFFSET  (0)
#define SHADER_COLOR_OFFSET   (1)

class RenderingViewer : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit RenderingViewer(QWidget *parent);
  ~RenderingViewer();

  QVector3D getCameraPosition() { return camera_pos_; }

  void setLightPosition(double x, double y, double z, int light_index) {
    if (light_index<0 || light_index>GlobalParams::POINTLIGHT_NUMBER-1) return;
    pointlights_[light_index]->setLightPosition(QVector3D(x, y, z));
  }
  void setLightColor(double r, double g, double b, int light_index) {
    if (light_index<0 || light_index>GlobalParams::POINTLIGHT_NUMBER-1) return;
    pointlights_[light_index]->setLightColor(QVector3D(r, g, b));
  }
  void setLightIntensity(double intensity, int light_index) {
    if (light_index<0 || light_index>GlobalParams::POINTLIGHT_NUMBER-1) return;
    pointlights_[light_index]->setLightItensity(intensity);
  }
  void setLightAmbient(double ambient) {
    PointLightModel::setLightAmbient(ambient);
  }

  void setObjectPosition(double x, double y, double z, int object_index) {
    if (object_index<0 || object_index>GlobalParams::OBJECT_NUMBER-1) return;
    objects_pose_[object_index].position = QVector3D(x, y, z);
  }
  void setObjectRotate(double roll, double pitch, double yaw, int object_index) {
    if (object_index<0 || object_index>GlobalParams::OBJECT_NUMBER-1) return;
    objects_pose_[object_index].rotate = QVector3D(roll, pitch, yaw);
  }
  void setObjectScale(double scale, int object_index) {
    if (object_index<0 || object_index>GlobalParams::OBJECT_NUMBER-1) return;
    objects_pose_[object_index].scale = scale;
  }

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;


 signals:
  void signalCameraPositionChange();

 public slots:
  void onChooseTextureImageOfObject1();
  void onChooseModelOfObject1();
  void onChooseTextureImageOfObject2();
  void onChooseModelOfObject2();
  void onChooseTextureImageOfObject3();
  void onChooseModelOfObject3();
  void onResetCameraPosition();

 private slots:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent*event);

 private:
  typedef struct {
    QVector3D position;
    QVector3D rotate;
    double scale;
  }ObjectPoseType;

 private:
  QOpenGLFunctions* fuc_;         // Qt封装的OpenGL函数对象

  QVector3D camera_pos_;
  QVector3D observe_center_;

  std::array<LightTextureModel*, GlobalParams::OBJECT_NUMBER> objects_;
  std::array<PointLightModel*, GlobalParams::POINTLIGHT_NUMBER> pointlights_;

  std::array<ObjectPoseType, GlobalParams::OBJECT_NUMBER> objects_pose_;

  struct {
    struct {
      bool is_pressed   = false;
      size_t last_pos_x = 0;
      size_t last_pos_y = 0;
    }left, right, mid;
  }mouse_;

  double aspect_ratio_ = 0;
};

#endif  // INC_RENDERING_VIEWER_H_
