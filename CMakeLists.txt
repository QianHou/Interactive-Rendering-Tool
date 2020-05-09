cmake_minimum_required (VERSION 2.8.3)
project (interactive_rendering_tool)

add_compile_options(-std=c++11)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
  include
  ${GLUT_INCLUDE_DIRS}
  ${OpenGL_INCLUDE_DIRS}
)

add_executable(interactive_rendering_tool src/main.cpp)
target_link_libraries(interactive_rendering_tool 
  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
)