/* Copyright [2020] <houqian & xiaotong> */
#include "inc/interactive_rendering_tool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  InteractiveRenderingTool tool;
  tool.show();
  return app.exec();
}
