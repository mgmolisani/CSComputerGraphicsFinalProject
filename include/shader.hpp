#ifndef CS5310_SHADER_HPP_
#define CS5310_SHADER_HPP_

#include <string>
#include "glad/glad.h"

class Shader {
 public:
  Shader(const GLenum shader_type, const std::string &source_file_path);
  ~Shader();

  const GLuint id() const;

 private:
  GLuint id_;
};

#endif  // CS5310_SHADER_HPP_
