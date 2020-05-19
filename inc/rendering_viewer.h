/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_RENDERING_VIEWER_H_
#define INC_RENDERING_VIEWER_H_

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class RenderingViewer : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit RenderingViewer(QWidget *parent);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

};

#endif  // INC_RENDERING_VIEWER_H_
