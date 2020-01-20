#ifndef CS5310_OBJECT_HPP_
#define CS5310_OBJECT_HPP_

#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"

class Object {
 public:
  Object();
  Object(const std::string& object_file_path);
  ~Object();

  const std::vector<GLfloat> vertices();
  const std::vector<GLuint> indices();

 private:
  struct Vertex {
    glm::vec3 position_;
    glm::vec3 normal_;

    Vertex(const glm::vec3& position, const glm::vec3& normal)
        : position_(position), normal_(normal){};
  };

  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
};

#endif  // CS5310_OBJECT_HPP_
