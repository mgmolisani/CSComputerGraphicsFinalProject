#include "object.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"

Object::Object()
    : vertices_({{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                 {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                 {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                 {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}}),
      indices_({0, 1, 3, 1, 2, 3}){};

Object::Object(const std::string& object_file_path) {
  std::ifstream object_file_stream(object_file_path);

  if (object_file_stream.is_open()) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;

    std::string line;
    while (std::getline(object_file_stream, line)) {
      std::istringstream line_stream(line);

      std::string key;
      line_stream >> key;

      if (key == "v") {
        std::string string_value;
        glm::vec3 position;

        for (int i = 0; i < 3; i++) {
          line_stream >> string_value;
          position[i] = std::stof(string_value);
        }

        positions.push_back(position);
      } else if (key == "vn") {
        std::string string_value;
        glm::vec3 normal;

        for (int i = 0; i < 3; i++) {
          line_stream >> string_value;
          normal[i] = std::stof(string_value);
        }

        normals.push_back(normal);
      } else if (key == "f") {
        std::string string_value;

        for (int i = 0; i < 3; i++) {
          line_stream >> string_value;

          std::string position_string =
              string_value.substr(0, string_value.find("/"));

          string_value = string_value.substr(position_string.length() + 1,
                                             string_value.length());

          std::string normal_string = string_value.substr(
              string_value.find("/") + 1, string_value.length());

          unsigned int position_index = std::stoul(position_string) - 1;
          unsigned int normal_index = std::stoul(normal_string) - 1;

          unsigned int current_vertex_index = 0;
          bool found = false;
          while (current_vertex_index < vertices_.size() && !found) {
            if (vertices_[current_vertex_index].position_ ==
                    positions[position_index] &&
                vertices_[current_vertex_index].normal_ ==
                    normals[normal_index]) {
              found = true;
            } else {
              current_vertex_index++;
            }
          }

          if (!found) {
            vertices_.push_back(
                Vertex(positions[position_index], normals[normal_index]));
          }

          indices_.push_back(current_vertex_index);
        }
      }
    }

    object_file_stream.close();
  } else {
    std::cout << "Could not open file at " << object_file_path << std::endl;
  }
}

Object::~Object() {}

const std::vector<GLfloat> Object::vertices() {
  unsigned int size = vertices_.size();
  std::vector<GLfloat> result;

  for (int i = 0; i < size; i++) {
    Vertex v = vertices_[i];

    result.push_back(v.position_[0]);
    result.push_back(v.position_[1]);
    result.push_back(v.position_[2]);

    result.push_back(v.normal_[0]);
    result.push_back(v.normal_[1]);
    result.push_back(v.normal_[2]);
  }

  return result;
}

const std::vector<GLuint> Object::indices() { return indices_; }
