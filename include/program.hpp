#ifndef CS5310_PROGRAM_HPP_
#define CS5310_PROGRAM_HPP_

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"

class Program {
 public:
  Program(const std::string &vertex_shader_source_file_path,
          const std::string &domain_shader_source_file_path,
          const std::string &geometry_shader_source_file_path,
          const std::string &fragment_shader_source_file_path);
  ~Program();

  void Use() const;
  void Validate() const;
  void SetUniformMatrix4f(const std::string &name,
                          const glm::mat4 &value) const;
  void SetUniform1f(const std::string &name, const GLfloat value) const;

 private:
  GLuint id_;
};

#endif  // CS5310_PROGRAM_HPP_
