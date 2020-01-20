#include "vertex_array.hpp"
#include "glad/glad.h"

VertexArray::VertexArray() { glGenVertexArrays(1, &id_); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id_); }

void VertexArray::Bind() const {
	glBindVertexArray(id_);
}

void VertexArray::AttribPointer(const GLuint index, const GLint size,
                                const GLenum type, const GLboolean normalized,
                                const GLsizei stride,
                                const GLvoid *const pointer) {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void VertexArray::EnableAttribArray(const GLuint index) {
  glEnableVertexAttribArray(index);
}
 
void VertexArray::DisableAttribArray(const GLuint index) {
  glDisableVertexAttribArray(index);
}