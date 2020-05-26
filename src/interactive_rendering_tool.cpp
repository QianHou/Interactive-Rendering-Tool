/* Copyright [2020] <houqian & xiaotong> */
#include "inc/interactive_rendering_tool.h"
#include "ui_interactive_rendering_tool.h"
#include <iostream>

InteractiveRenderingTool::InteractiveRenderingTool(QWidget *parent) :
  QWidget(parent),
  ui_(new Ui::InteractiveRenderingTool),
  viewer_(new RenderingViewer(this)) {
  ui_->setupUi(this);
  this->uiParamsInit();

  this->setFixedSize(this->width(), this->height());
  viewer_->setObjectName(QStringLiteral("RenderingViewer"));
  viewer_->setGeometry(QRect(0, 0, ui_->viewer_divide->x(), this->geometry().height()));

  connect(viewer_, SIGNAL(signalObjectNameChange()), this, SLOT(onObjectNameChange()));

  connect(ui_->object1_model_button, SIGNAL(clicked()), viewer_, SLOT(onChooseModelOfObject1()), Qt::DirectConnection);
  connect(ui_->object1_texture_button, SIGNAL(clicked()), viewer_, SLOT(onChooseTextureImageOfObject1()), Qt::DirectConnection);
  connect(ui_->object1_x_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object1_y_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object1_z_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object1_roll_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object1_pitch_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object1_yaw_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object1_scale_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectScaleChange(double)));

  connect(ui_->object2_model_button, SIGNAL(clicked()), viewer_, SLOT(onChooseModelOfObject2()), Qt::DirectConnection);
  connect(ui_->object2_texture_button, SIGNAL(clicked()), viewer_, SLOT(onChooseTextureImageOfObject2()), Qt::DirectConnection);
  connect(ui_->object2_x_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object2_y_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object2_z_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object2_roll_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object2_pitch_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object2_yaw_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object2_scale_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectScaleChange(double)));

  connect(ui_->object3_model_button, SIGNAL(clicked()), viewer_, SLOT(onChooseModelOfObject3()), Qt::DirectConnection);
  connect(ui_->object3_texture_button, SIGNAL(clicked()), viewer_, SLOT(onChooseTextureImageOfObject3()), Qt::DirectConnection);
  connect(ui_->object3_x_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object3_y_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object3_z_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectPositionChange(double)));
  connect(ui_->object3_roll_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object3_pitch_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object3_yaw_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectRotateChange(double)));
  connect(ui_->object3_scale_setbox, SIGNAL(valueChanged(double)), this, SLOT(onObjectScaleChange(double)));

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

  connect(ui_->light_ambient_slider, SIGNAL(valueChanged(int)), this , SLOT(onLightAmbientChange(int)));
  connect(ui_->campos_reset_button, SIGNAL(clicked()), viewer_, SLOT(onResetCameraPosition()), Qt::DirectConnection);

  connect(viewer_, SIGNAL(signalCameraPositionChange()), this, SLOT(onCameraPositionChange()));
}

InteractiveRenderingTool::~InteractiveRenderingTool() {
  delete ui_;
  delete viewer_;
}

void InteractiveRenderingTool::uiParamsInit() {
  ui_->object1_name->setText(GlobalParams::OBJECT1_OBJ_PATH_INIT.split('/').last());
  ui_->object1_x_setbox->setValue(GlobalParams::OBJECT1_POSITION_INIT.x());
  ui_->object1_y_setbox->setValue(GlobalParams::OBJECT1_POSITION_INIT.y());
  ui_->object1_z_setbox->setValue(GlobalParams::OBJECT1_POSITION_INIT.z());
  ui_->object1_roll_setbox->setValue(GlobalParams::OBJECT1_ROTATE_INIT.x());
  ui_->object1_pitch_setbox->setValue(GlobalParams::OBJECT1_ROTATE_INIT.y());
  ui_->object1_yaw_setbox->setValue(GlobalParams::OBJECT1_ROTATE_INIT.z());
  ui_->object1_scale_setbox->setValue(GlobalParams::OBJECT1_SCALE_INIT);

  ui_->object2_name->setText(GlobalParams::OBJECT2_OBJ_PATH_INIT.split('/').last());
  ui_->object2_x_setbox->setValue(GlobalParams::OBJECT2_POSITION_INIT.x());
  ui_->object2_y_setbox->setValue(GlobalParams::OBJECT2_POSITION_INIT.y());
  ui_->object2_z_setbox->setValue(GlobalParams::OBJECT2_POSITION_INIT.z());
  ui_->object2_roll_setbox->setValue(GlobalParams::OBJECT2_ROTATE_INIT.x());
  ui_->object2_pitch_setbox->setValue(GlobalParams::OBJECT2_ROTATE_INIT.y());
  ui_->object2_yaw_setbox->setValue(GlobalParams::OBJECT2_ROTATE_INIT.z());
  ui_->object2_scale_setbox->setValue(GlobalParams::OBJECT2_SCALE_INIT);

  ui_->object3_name->setText(GlobalParams::OBJECT3_OBJ_PATH_INIT.split('/').last());
  ui_->object3_x_setbox->setValue(GlobalParams::OBJECT3_POSITION_INIT.x());
  ui_->object3_y_setbox->setValue(GlobalParams::OBJECT3_POSITION_INIT.y());
  ui_->object3_z_setbox->setValue(GlobalParams::OBJECT3_POSITION_INIT.z());
  ui_->object3_roll_setbox->setValue(GlobalParams::OBJECT3_ROTATE_INIT.x());
  ui_->object3_pitch_setbox->setValue(GlobalParams::OBJECT3_ROTATE_INIT.y());
  ui_->object3_yaw_setbox->setValue(GlobalParams::OBJECT3_ROTATE_INIT.z());
  ui_->object3_scale_setbox->setValue(GlobalParams::OBJECT3_SCALE_INIT);

  ui_->light1_x_setbox->setValue(GlobalParams::POINTLIGHT1_POSITION_INIT.x());
  ui_->light1_y_setbox->setValue(GlobalParams::POINTLIGHT1_POSITION_INIT.y());
  ui_->light1_z_setbox->setValue(GlobalParams::POINTLIGHT1_POSITION_INIT.z());
  ui_->light1_r_setbox->setValue(GlobalParams::POINTLIGHT1_COLOR_INIT.x());
  ui_->light1_g_setbox->setValue(GlobalParams::POINTLIGHT1_COLOR_INIT.y());
  ui_->light1_b_setbox->setValue(GlobalParams::POINTLIGHT1_COLOR_INIT.z());
  ui_->light1_itensity_setbox->setValue(GlobalParams::POINTLIGHT1_ITENSITY_INIT);

  ui_->light2_x_setbox->setValue(GlobalParams::POINTLIGHT2_POSITION_INIT.x());
  ui_->light2_y_setbox->setValue(GlobalParams::POINTLIGHT2_POSITION_INIT.y());
  ui_->light2_z_setbox->setValue(GlobalParams::POINTLIGHT2_POSITION_INIT.z());
  ui_->light2_r_setbox->setValue(GlobalParams::POINTLIGHT2_COLOR_INIT.x());
  ui_->light2_g_setbox->setValue(GlobalParams::POINTLIGHT2_COLOR_INIT.y());
  ui_->light2_b_setbox->setValue(GlobalParams::POINTLIGHT2_COLOR_INIT.z());
  ui_->light2_itensity_setbox->setValue(GlobalParams::POINTLIGHT2_ITENSITY_INIT);

  ui_->light_ambient_slider->setValue(GlobalParams::AMBIENT_LIGHT_INIT);

  ui_->campos_x_label->setText(QString::number(GlobalParams::CAMERA_POSITION_INIT.x(), 'f', 2));
  ui_->campos_y_label->setText(QString::number(GlobalParams::CAMERA_POSITION_INIT.y(), 'f', 2));
  ui_->campos_z_label->setText(QString::number(GlobalParams::CAMERA_POSITION_INIT.z(), 'f', 2));
}

void InteractiveRenderingTool::onObjectNameChange() {
  ui_->object1_name->setText(viewer_->getObjectName(0));
  ui_->object2_name->setText(viewer_->getObjectName(1));
  ui_->object3_name->setText(viewer_->getObjectName(2));
}

void InteractiveRenderingTool::onObjectPositionChange(double value) {
  viewer_->setObjectPosition(ui_->object1_x_setbox->value(), ui_->object1_y_setbox->value(), ui_->object1_z_setbox->value(), 0);
  viewer_->setObjectPosition(ui_->object2_x_setbox->value(), ui_->object2_y_setbox->value(), ui_->object2_z_setbox->value(), 1);
  viewer_->setObjectPosition(ui_->object3_x_setbox->value(), ui_->object3_y_setbox->value(), ui_->object3_z_setbox->value(), 2);
}

void InteractiveRenderingTool::onObjectRotateChange(double value) {
  viewer_->setObjectRotate(ui_->object1_roll_setbox->value(), ui_->object1_pitch_setbox->value(), ui_->object1_yaw_setbox->value(), 0);
  viewer_->setObjectRotate(ui_->object2_roll_setbox->value(), ui_->object2_pitch_setbox->value(), ui_->object2_yaw_setbox->value(), 1);
  viewer_->setObjectRotate(ui_->object3_roll_setbox->value(), ui_->object3_pitch_setbox->value(), ui_->object3_yaw_setbox->value(), 2);
}

void InteractiveRenderingTool::onObjectScaleChange(double value) {
  viewer_->setObjectScale(ui_->object1_scale_setbox->value(), 0);
  viewer_->setObjectScale(ui_->object2_scale_setbox->value(), 1);
  viewer_->setObjectScale(ui_->object3_scale_setbox->value(), 2);
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

void InteractiveRenderingTool::onCameraPositionChange() {
  QVector3D position = viewer_->getCameraPosition();
  ui_->campos_x_label->setText(QString::number(position.x(), 'f', 2));
  ui_->campos_y_label->setText(QString::number(position.y(), 'f', 2));
  ui_->campos_z_label->setText(QString::number(position.z(), 'f', 2));
}
