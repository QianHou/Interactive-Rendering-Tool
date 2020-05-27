/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_OBJECT_LOADER_H_
#define INC_OBJECT_LOADER_H_

#include <QOpenGLFunctions>
#include <QString>
#include <vector>
#include <string>

class ObjectLoader {
 public:
  typedef struct {
    GLfloat* buffer = NULL;
    int size;
  } ObjectBufferType;

 public:
  ObjectLoader(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data, const std::vector<GLfloat>& normal_vector_data);
  ObjectLoader(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data);
  explicit ObjectLoader(const QString & obj_file_path);

  void reloadObject(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data, const std::vector<GLfloat>& normal_vector_data);
  void reloadObject(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data);
  void reloadObject(const QString & obj_file_path);
  void reloadObject(const std::string& obj_file_path);

  void bufferClear();

 public:
  ObjectBufferType vertex_, normal_vector_, texture_index_;

 private:
  void calcNormalVector(const std::vector<GLfloat>& vertex_data);
};

namespace ObjectData {
  const std::vector<GLfloat> TETRAHEDRO_VERTEX = {
    // surface 1
    -0.5f, 0.0f, -0.2887f,
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    // surface 2
    0.5f, 0.0f, -0.2887f,
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    // surface 3
    -0.5f, 0.0f, -0.2887f,
    0.0f, 0.0f, 0.5774f,
    0.0f, 0.8165f, 0.0f,
    // surface 4
    0.5f, 0.0f, -0.2887f,
    0.0f, 0.8165f, 0.0f,
    0.0f, 0.0f, 0.5774f,
  };

  const std::vector<GLfloat> TETRAHEDRO_TEXTURE_INDEX = {
    // surface 1
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    // surface 2
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    // surface 3
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    // surface 4
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  };

  const std::vector<GLfloat> SQUARE_VERTEX = {
    // surface 1
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    // surface 2
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
  };

  const std::vector<GLfloat> SQUARE_TEXTURE_INDEX = {
    // surface 1
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    // surface 2
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  };
};

#endif  // INC_OBJECT_LOADER_H_
