/* Copyright [2020] <houqian & xiaotong> */
#include "inc/interactive_rendering_tool.h"
#include "ui_interactive_rendering_tool.h"
#include <iostream>

InteractiveRenderingTool::InteractiveRenderingTool(QWidget *parent) :
  QWidget(parent),
  ui_(new Ui::InteractiveRenderingTool),
  viewer_(new RenderingViewer(this)) {
  ui_->setupUi(this);

  this->setFixedSize(this->width(), this->height());
  viewer_->setObjectName(QStringLiteral("RenderingViewer"));
  viewer_->setGeometry(QRect(0, 0, ui_->viewer_divide->x(), this->geometry().height()));

  connect(ui_->texture_choose_button, SIGNAL(clicked()), viewer_, SLOT(onChooseTextureImage()), Qt::DirectConnection);
  connect(ui_->campos_reset_button, SIGNAL(clicked()), viewer_, SLOT(onResetCameraPosition()), Qt::DirectConnection);
  connect(ui_->light_ambient_slider, SIGNAL(valueChanged(int)), this , SLOT(onLightAmbientChange(int)));

  connect(ui_->light1_x_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightPositionChange(double)));
  connect(ui_->light1_y_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightPositionChange(double)));
  connect(ui_->light1_z_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightPositionChange(double)));
  connect(ui_->light1_r_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightColorChange(double)));
  connect(ui_->light1_g_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightColorChange(double)));
  connect(ui_->light1_b_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightColorChange(double)));
  connect(ui_->light1_itensity_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightIntensityChange(double)));

  connect(ui_->light2_x_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightPositionChange(double)));
  connect(ui_->light2_y_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightPositionChange(double)));
  connect(ui_->light2_z_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightPositionChange(double)));
  connect(ui_->light2_r_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightColorChange(double)));
  connect(ui_->light2_g_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightColorChange(double)));
  connect(ui_->light2_b_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightColorChange(double)));
  connect(ui_->light2_itensity_setbox, SIGNAL(valueChanged(double)), this, SLOT(onLightIntensityChange(double)));

  connect(viewer_, SIGNAL(signalCameraPositionChange()), this, SLOT(onCameraPositionChange()));
}

InteractiveRenderingTool::~InteractiveRenderingTool() {
  delete ui_;
  delete viewer_;
}

void InteractiveRenderingTool::onCameraPositionChange() {
  QVector3D position = viewer_->getCameraPosition();
  ui_->campos_x_label->setText(QString::number(position.x(), 'f', 2));
  ui_->campos_y_label->setText(QString::number(position.y(), 'f', 2));
  ui_->campos_z_label->setText(QString::number(position.z(), 'f', 2));
}

void InteractiveRenderingTool::onLightAmbientChange(int value) {
  viewer_->setLightAmbient(value / 100.0f);
}

void InteractiveRenderingTool::onLightPositionChange(double value) {
  viewer_->setLightPosition(ui_->light1_x_setbox->value(), ui_->light1_y_setbox->value(), ui_->light1_z_setbox->value(), 0);
  viewer_->setLightPosition(ui_->light2_x_setbox->value(), ui_->light2_y_setbox->value(), ui_->light2_z_setbox->value(), 1);
}

void InteractiveRenderingTool::onLightColorChange(double value) {
  viewer_->setLightColor(ui_->light1_r_setbox->value(), ui_->light1_g_setbox->value(), ui_->light1_b_setbox->value(), 0);
  viewer_->setLightColor(ui_->light2_r_setbox->value(), ui_->light2_g_setbox->value(), ui_->light2_b_setbox->value(), 1);
}

void InteractiveRenderingTool::onLightIntensityChange(double value) {
  viewer_->setLightIntensity(ui_->light1_itensity_setbox->value(), 0);
  viewer_->setLightIntensity(ui_->light2_itensity_setbox->value(), 1);
}


