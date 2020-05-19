/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_INTERACTIVE_RENDERING_TOOL_H_
#define INC_INTERACTIVE_RENDERING_TOOL_H_

#include <QWidget>
#include "inc/rendering_viewer.h"

namespace Ui {
class InteractiveRenderingTool;
}


class InteractiveRenderingTool : public QWidget {
  Q_OBJECT

 public:
  explicit InteractiveRenderingTool(QWidget *parent = nullptr);
  ~InteractiveRenderingTool();

 private:
  Ui::InteractiveRenderingTool* ui_;
  RenderingViewer* viewer_;
};

#endif  // INC_INTERACTIVE_RENDERING_TOOL_H_
