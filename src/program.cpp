#include "program.hpp"
#include <iostream>
#include "glad/glad.h"
#include "shader.hpp"

Program::Program(const std::string &vertex_shader_source_file_path,
                 const std::string &domain_shader_source_file_path,
                 const std::string &geometry_shader_source_file_path,
                 const std::string &fragment_shader_source_file_path) {
  id_ = glCreateProgram();

  Shader vertex_shader(GL_VERTEX_SHADER, vertex_shader_source_file_path);
  Shader domain_shader(GL_TESS_EVALUATION_SHADER,
                       domain_shader_source_file_path);
  Shader geometry_shader(GL_GEOMETRY_SHADER, geometry_shader_source_file_path);
  Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_shader_source_file_path);

  GLuint vertex_shader_id = vertex_shader.id();
  GLuint domain_shader_id = domain_shader.id();
  GLuint geometry_shader_id = geometry_shader.id();
  GLuint fragment_shader_id = fragment_shader.id();

  glAttachShader(id_, vertex_shader_id);
  glAttachShader(id_, domain_shader_id);
  glAttachShader(id_, geometry_shader_id);
  glAttachShader(id_, fragment_shader_id);

  GLint success;
  glLinkProgram(id_);

  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (success) {
    std::cout << "Program " << id_ << " linking successful!" << std::endl;
  } else {
    std::cout << "Program " << id_ << " linking failed!" << std::endl;

    GLint length;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);

    if (length) {
      GLchar *info_log = new GLchar[length];
      glGetProgramInfoLog(id_, (GLsizei)length, NULL, info_log);

      std::cout << (char *)info_log << std::endl;

      delete[] info_log;
    } else {
      std::cout << "No information was logged for the previous linking attempt "
                   "for Program "
                << id_ << "." << std::endl;
    }
  }

  glDetachShader(id_, vertex_shader_id);
  glDetachShader(id_, domain_shader_id);
  glDetachShader(id_, geometry_shader_id);
  glDetachShader(id_, fragment_shader_id);
}

Program::~Program() { glDeleteProgram(id_); }

void Program::Use() const { glUseProgram(id_); }

void Program::Validate() const {
  GLint success;
  glValidateProgram(id_);

  glGetProgramiv(id_, GL_VALIDATE_STATUS, &success);
  if (success) {
    std::cout << "Program " << id_ << " validation successful!" << std::endl;
  } else {
    std::cout << "Program " << id_ << " validation failed!" << std::endl;

    GLint length;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);

    if (length) {
      GLchar *info_log = new GLchar[length];
      glGetProgramInfoLog(id_, (GLsizei)length, NULL, info_log);

      std::cout << (char *)info_log << std::endl;

      delete[] info_log;
    } else {
      std::cout << "No information was logged for the previous validation "
                   "attempt for Program "
                << id_ << "." << std::endl;
    }
  }
}

void Program::SetUniformMatrix4f(const std::string &name,
                                 const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void Program::SetUniform1f(const std::string &name, const GLfloat value) const {
  glUniform1fv(glGetUniformLocation(id_, name.c_str()), 1, &value);
}