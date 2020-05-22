/* Copyright [2020] <houqian & xiaotong> */
#include "inc/object_loader.h"
#include <iostream>

ObjectLoader::ObjectLoader(std::vector<GLfloat> vertex_data, std::vector<GLfloat> texture_index_data) {
  this->reloadObject(vertex_data, texture_index_data);
}

void ObjectLoader::reloadObject(std::vector<GLfloat> vertex_data, std::vector<GLfloat> texture_index_data) {
  if (vertex_data.size()%9 != 0 || texture_index_data.size()%6 != 0 || vertex_data.size()/9 != texture_index_data.size()/6) {
    std::cout << "invalid data length" << std::endl;
    return;
  }

  // resolve vertex
  if (!vertex_data.empty()) {
    vertex_.size = vertex_data.size();
    vertex_.buffer = new GLfloat[vertex_.size];
    std::copy(vertex_data.begin(), vertex_data.end(), vertex_.buffer);
  }

  // resolve normal vector
  if (!vertex_data.empty()) {
    normal_vector_.size = vertex_data.size();
    normal_vector_.buffer = new GLfloat[normal_vector_.size];
    for (size_t i =0 ; i < normal_vector_.size ; i= i+9) {
      std::array<GLfloat, 3> point1, point2, point3;
      std::copy(vertex_data.begin()+ i+0, vertex_data.begin()+ i+3, point1.begin());
      std::copy(vertex_data.begin()+ i+3, vertex_data.begin()+ i+6, point2.begin());
      std::copy(vertex_data.begin()+ i+6, vertex_data.begin()+ i+9, point3.begin());

      std::array<GLfloat, 3> vector1, vector2;
      for (size_t j = 0 ; j < 3 ; j++) {
        vector1[j] = point3[j] - point1[j];
        vector2[j] = point1[j] - point2[j];
      }

      std::array<GLfloat, 3> normal_vertex;
      normal_vertex[0] = vector1[1] * vector2[2] - vector2[1] * vector1[2];
      normal_vertex[1] = vector1[2] * vector2[0] - vector2[2] * vector1[0];
      normal_vertex[2] = vector1[0] * vector2[1] - vector2[0] * vector1[1];

      std::copy(normal_vertex.begin(), normal_vertex.end(), normal_vector_.buffer+ i+0);
      std::copy(normal_vertex.begin(), normal_vertex.end(), normal_vector_.buffer+ i+3);
      std::copy(normal_vertex.begin(), normal_vertex.end(), normal_vector_.buffer+ i+6);
    }
  }

  // resolve texture_index
  if (!texture_index_data.empty()) {
    texture_index_.size = texture_index_data.size();
    texture_index_.buffer = new GLfloat[texture_index_.size];
    std::copy(texture_index_data.begin(), texture_index_data.end(), texture_index_.buffer);
  }
}