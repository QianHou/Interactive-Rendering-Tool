/* Copyright [2020] <houqian & xiaotong> */
#include <GL/glut.h>
#include "../include/interactive_rendering_tool.h"

void draw(void) {
  // green background
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

int main(int argc, char *argv[])
{
  /* GLUT环境初始化*/
  glutInit(&argc, argv);
  /* 显示模式初始化 */
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  /* 窗口大小定义 */
  glutInitWindowSize(480, 480);
  /* 定义窗口位置 */
  glutInitWindowPosition(0, 0);
  /* 窗口显示 */
  glutCreateWindow("Interactive Rendering Tool");
  /* 注册绘图函数 */
  glutDisplayFunc(draw);
  /* GLUT消息循环，开始执行程序 */
  glutMainLoop();
  return 0;
}