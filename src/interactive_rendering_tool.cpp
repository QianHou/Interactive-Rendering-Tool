/* Copyright [2020] <houqian & xiaotong> */
#include "inc/interactive_rendering_tool.h"
#include "ui_interactive_rendering_tool.h"

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

