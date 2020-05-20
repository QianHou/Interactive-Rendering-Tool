cmake_minimum_required (VERSION 2.8.3)
project (interactive_rendering_tool)

add_compile_options(-std=c++11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_USE_QTOPENGL TRUE)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS
  Core
  Widgets
  Gui
  OpenGL
  REQUIRED
)


include_directories(
  inc
  ${GLUT_INCLUDE_DIRS}
  ${OpenGL_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5OpenGL_LIBRARIES}
)

qt5_wrap_cpp(MOC_LIBS
  inc/interactive_rendering_tool.h
  inc/rendering_viewer.h
)
qt5_add_resources(QRC_LIBS
  resource/res.qrc
)

add_executable(interactive_rendering_tool
  src/main.cpp
  src/interactive_rendering_tool.cpp
  src/rendering_viewer.cpp
  ${MOC_LIBS}
  ${QRC_LIBS}
)

target_link_libraries(interactive_rendering_tool
  ${GLUT_INCLUDE_DIRS}
  ${OpenGL_INCLUDE_DIRS}
  Qt5::Widgets
  Qt5::OpenGL
)