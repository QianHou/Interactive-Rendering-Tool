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
}

InteractiveRenderingTool::~InteractiveRenderingTool() {
  delete ui_;
  delete viewer_;
}

