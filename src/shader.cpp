#include "shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "glad/glad.h"

Shader::Shader(const GLenum shader_type, const std::string &source_file_path) {
  id_ = glCreateShader(shader_type);

  std::ifstream source_file_stream(source_file_path);

  if (source_file_stream.is_open()) {
    std::ostringstream source_string_stream;

    source_string_stream << source_file_stream.rdbuf();
    const std::string source_string = source_string_stream.str();
    const GLchar *const source_c_string = (GLchar *)source_string.c_str();

    glShaderSource(id_, 1, &source_c_string, NULL);
    glCompileShader(id_);

    GLint success;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (success) {
      std::cout << "Shader for " << source_file_path
                << " compilation successful!" << std::endl;
    } else {
      std::cout << "Shader for " << source_file_path << " compilation failed!"
                << std::endl;

      GLint length;
      glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);

      if (length) {
        GLchar *info_log = new GLchar[length];
        glGetShaderInfoLog(id_, (GLsizei)length, NULL, info_log);

        std::cout << (char *)info_log << std::endl;

        delete[] info_log;
      } else {
        std::cout << "No information was logged for the previous compilation "
                     "attempt for "
                  << source_file_path << "." << std::endl;
      }
    }

    source_file_stream.close();
  }
}

Shader::~Shader() { glDeleteShader(id_); }

const GLuint Shader::id() const { return id_; }