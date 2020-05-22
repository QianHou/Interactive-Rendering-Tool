/* Copyright [2020] <houqian & xiaotong> */
#include "inc/object_loader.h"
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QByteArray>
#include <iostream>

ObjectLoader::ObjectLoader(const std::vector<GLfloat>& vertex_data,
                           const std::vector<GLfloat>& texture_index_data,
                           const std::vector<GLfloat>& normal_vector_data) {
  this->reloadObject(vertex_data, texture_index_data, normal_vector_data);
}

ObjectLoader::ObjectLoader(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data) {
  this->reloadObject(vertex_data, texture_index_data);
}

ObjectLoader::ObjectLoader(const QString & obj_file_path) {
  this->reloadObject(obj_file_path);
}

void ObjectLoader::reloadObject(const std::vector<GLfloat>& vertex_data,
                                const std::vector<GLfloat>& texture_index_data,
                                const std::vector<GLfloat>& normal_vector_data) {
  if (vertex_data.size()%9 != 0 || texture_index_data.size()%6 != 0) {
    std::cout << "[ERROR] invalid data length" << std::endl;
    return;
  }

  // resolve vertex
  if (!vertex_data.empty()) {
    vertex_.size = vertex_data.size();

    if (vertex_.buffer) delete vertex_.buffer;
    vertex_.buffer = new GLfloat[vertex_.size];

    std::copy(vertex_data.begin(), vertex_data.end(), vertex_.buffer);
  } else {
    vertex_.size = 0;
    vertex_.buffer = NULL;
  }

  // resolve normal vector
  if (vertex_data.size() == normal_vector_data.size()) {
    normal_vector_.size = normal_vector_data.size();

    if (normal_vector_.buffer) delete normal_vector_.buffer;
    normal_vector_.buffer = new GLfloat[normal_vector_.size];

    std::copy(normal_vector_data.begin(), normal_vector_data.end(), normal_vector_.buffer);
  } else {
    // 法向量维数与顶点数不符，放弃使用传入的法向量
    this->calcNormalVector(vertex_data);
  }

  // resolve texture index, make sure the buffer size is equle to vertex_data.size() /9*6
  if (!texture_index_data.empty()) {
    texture_index_.size = vertex_data.size() /9*6;

    if (texture_index_.buffer) delete texture_index_.buffer;
    texture_index_.buffer = new GLfloat[texture_index_.size];

    if (texture_index_.size > texture_index_data.size()) {
      std::copy(texture_index_data.begin(), texture_index_data.end(), texture_index_.buffer);
      // 缺少的纹理映射信息，用默认纹理映射补全
      for (size_t i = texture_index_data.size(); i < texture_index_.size ; i=i+6) {
        std::array<GLfloat, 6> default_texture = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        std::copy(default_texture.begin(), default_texture.end(), texture_index_.buffer + i);
      }
    } else {
      std::copy(texture_index_data.begin(), texture_index_data.begin() + texture_index_.size, texture_index_.buffer);
    }

  } else {
    // 纹理数据输入为空
    if (vertex_data.empty()) {
      texture_index_.size = 0;
      texture_index_.buffer = NULL;
    } else {
      // 顶点数据非空，按照顶点数量补全默认纹理映射
      texture_index_.size = vertex_data.size() /9*6;

      if (texture_index_.buffer) delete texture_index_.buffer;
      texture_index_.buffer = new GLfloat[texture_index_.size];

      for (size_t i = 0; i < texture_index_.size ; i=i+6) {
        std::array<GLfloat, 6> default_texture = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        std::copy(default_texture.begin(), default_texture.end(), texture_index_.buffer + i);
      }
    }
  }
}

void ObjectLoader::reloadObject(const std::vector<GLfloat>& vertex_data, const std::vector<GLfloat>& texture_index_data) {
  std::vector<GLfloat> normal_vector_data;
  this->reloadObject(vertex_data, texture_index_data, normal_vector_data);
}

void ObjectLoader::reloadObject(const QString & obj_file_path) {
  if (obj_file_path.mid(obj_file_path.lastIndexOf('.')) != ".obj") {
    std::cout << "[ERROR] obj file only" << std::endl;
    return;
  }

  QFile obj_file(obj_file_path);
  if (!obj_file.open(QIODevice::ReadOnly)) {
    std::cout << "[ERROR] open " << obj_file_path.toStdString() << " failed" << std::endl;
    return;
  }

  // file reading
  std::vector<std::array<GLfloat, 3>> points;               // 顶点原始数据
  std::vector<std::array<GLfloat, 2>> textures;             // 纹理索引原始数据
  std::vector<std::array<GLfloat, 3>> normal_vecs;          // 法向量原始数据
  std::vector<std::array<int, 3>> surface_points_index;     // 三角形顶点原始数据
  std::vector<std::array<int, 3>> surface_textures_index;   // 三角形纹理索引原始数据
  std::vector<std::array<int, 3>> surface_normals_index;   // 三角形纹理索引原始数据

  QTextStream texts(&obj_file);
  while (!texts.atEnd()) {
    QString line_str = texts.readLine();
    if (line_str.isEmpty()) continue;

    QList<QString> line_list = line_str.split(' ');
    line_list.removeAll("");

    QString data_type = line_list.takeFirst();
    if (line_list.isEmpty()) continue;

    if (data_type == "v") {
      // 顶点读取
      if (line_list.size() == 3) {
        points.push_back({line_list[0].toFloat(), line_list[1].toFloat(), line_list[2].toFloat()});
      }

    } else if (data_type == "vt") {
      // 纹理索引读取
      if (line_list.size() == 2 || line_list.size() == 3) {
        textures.push_back({line_list[0].toFloat(), line_list[1].toFloat()});
      }
    } else if (data_type == "vn") {
      // 法向量读取
      if (line_list.size() == 3) {
        normal_vecs.push_back({line_list[0].toFloat(), line_list[1].toFloat(), line_list[2].toFloat()});
      }
    } else if (data_type == "f") {
      std::vector<std::tuple<int, int, int>> line_indexes;
      std::transform(line_list.begin(), line_list.end(), std::back_inserter(line_indexes),
                      [](const QString line_item) {
                      QList<QString> index_list = line_item.split('/');
                      if (index_list.size() == 2)
                        return std::make_tuple(index_list[0].toInt(), index_list[1].toInt(), -1);
                      else
                        return std::make_tuple(index_list[0].toInt(), index_list[1].toInt(), index_list[2].toInt());
                      });

      if (line_list.size() == 3) {
        // 三角形平面读取
        surface_points_index.push_back({std::get<0>(line_indexes[0]), std::get<0>(line_indexes[1]), std::get<0>(line_indexes[2])});
        surface_textures_index.push_back({std::get<1>(line_indexes[0]), std::get<1>(line_indexes[1]), std::get<1>(line_indexes[2])});
        surface_normals_index.push_back({std::get<2>(line_indexes[0]), std::get<2>(line_indexes[1]), std::get<2>(line_indexes[2])});
      } else if (line_list.size() == 4) {
        // 四边形平面读取
        surface_points_index.push_back({std::get<0>(line_indexes[0]), std::get<0>(line_indexes[1]), std::get<0>(line_indexes[2])});
        surface_points_index.push_back({std::get<0>(line_indexes[2]), std::get<0>(line_indexes[3]), std::get<0>(line_indexes[0])});

        surface_textures_index.push_back({std::get<1>(line_indexes[0]), std::get<1>(line_indexes[1]), std::get<1>(line_indexes[2])});
        surface_textures_index.push_back({std::get<1>(line_indexes[2]), std::get<1>(line_indexes[3]), std::get<1>(line_indexes[0])});

        surface_normals_index.push_back({std::get<2>(line_indexes[0]), std::get<2>(line_indexes[1]), std::get<2>(line_indexes[2])});
        surface_normals_index.push_back({std::get<2>(line_indexes[2]), std::get<2>(line_indexes[3]), std::get<2>(line_indexes[0])});
      }
    }
  }
  obj_file.close();

  // data process
  std::vector<GLfloat> vertex_data;
  std::vector<GLfloat> texture_index_data;
  std::vector<GLfloat> normal_vector_data;

  if (!textures.empty()) {
    for (const auto indexes : surface_points_index) {
      for (const auto index : indexes) {
        vertex_data.push_back(points[index][0]);
        vertex_data.push_back(points[index][1]);
        vertex_data.push_back(points[index][2]);
      }
    }
  } else {
    std::cout << "[WARN] no surface in " << obj_file_path.toStdString() << std::endl;
  }

  if (!textures.empty()) {
    for (const auto indexes : surface_textures_index) {
      for (const auto index : indexes) {
        texture_index_data.push_back(textures[index][0]);
        texture_index_data.push_back(textures[index][1]);
      }
    }
  } else {
    std::cout << "[WARN] no texture index in " << obj_file_path.toStdString() << std::endl;
  }

  if (!normal_vecs.empty()) {
    for (const auto indexes : surface_normals_index) {
      for (const auto index : indexes) {
        normal_vector_data.push_back(normal_vecs[index][0]);
        normal_vector_data.push_back(normal_vecs[index][1]);
        normal_vector_data.push_back(normal_vecs[index][2]);
      }
    }
  } else {
    std::cout << "[WARN] no nomal vector in " << obj_file_path.toStdString() << std::endl;
  }

  this->reloadObject(vertex_data, texture_index_data, normal_vector_data);
}

void ObjectLoader::reloadObject(const std::string& obj_file_path) {
  this->reloadObject(QString::fromStdString(obj_file_path));
}

void ObjectLoader::bufferClear() {
  vertex_.size = 0;
  vertex_.buffer = NULL;
  normal_vector_.size = 0;
  normal_vector_.buffer = NULL;
  texture_index_.size = 0;
  texture_index_.buffer = NULL;
}

void ObjectLoader::calcNormalVector(const std::vector<GLfloat>& vertex_data) {
  if (!vertex_data.empty()) {
    std::cout << "[INFO] calcuate normal vevtor by vertex" << std::endl;

    normal_vector_.size = vertex_data.size();

    if (normal_vector_.buffer) delete normal_vector_.buffer;
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
  } else {
    normal_vector_.size = 0;
    normal_vector_.buffer = NULL;
  }
}