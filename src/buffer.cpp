#include "buffer.hpp"

Buffer::Buffer() { glGenBuffers(1, &id_); }

Buffer::~Buffer() { glDeleteBuffers(1, &id_); }

void Buffer::Bind(const GLenum target) const { glBindBuffer(target, id_); }

void Buffer::Data(const GLenum target, const GLsizeiptr size,
                  const GLvoid *const data, GLenum usage) {
  glBufferData(target, size, data, usage);
}