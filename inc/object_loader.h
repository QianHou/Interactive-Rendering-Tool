/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_OBJECT_LOADER_H_
#define INC_OBJECT_LOADER_H_

#include <QOpenGLFunctions>
#include <QString>
#include <vector>
#include <string>

class ObjectLoader {
 public:
  ObjectLoader(std::vector<GLfloat> vertex_data, std::vector<GLfloat> texture_index_data);

  void reloadObject(std::vector<GLfloat> vertex_data, std::vector<GLfloat> texture_index_data);
  void reloadObject(std::string obj_file_path);
  void reloadObject(QString obj_file_path);

 public:
  struct {
    GLfloat* buffer = NULL;
    int size;
  }vertex_, normal_vector_, texture_index_;
};

namespace ObjectData {
  const std::vector<GLfloat> TETRAHEDRO_VERTEX = {
    // face 1
    -0.5f, 0.0f, -0.2887f,
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    // face 2
    0.5f, 0.0f, -0.2887f,
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    // face 3
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    0.0f, 0.8165f, 0.0f,
    // face 4
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    0.0f, 0.0f, 0.5774f,
  };

  const std::vector<GLfloat> TETRAHEDRO_TEXTURE_INDEX = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  };
};

#endif  // INC_OBJECT_LOADER_H_
