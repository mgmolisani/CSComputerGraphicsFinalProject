#ifndef CS5310_BUFFER_HPP_
#define CS5310_BUFFER_HPP_

#include "glad/glad.h"

class Buffer {
 public:
  Buffer();
  ~Buffer();

	void Bind(const GLenum target) const;

	static void Data(const GLenum target, const GLsizeiptr size, const GLvoid *const data, GLenum usage);

 private:
  GLuint id_;
};

#endif  // CS5310_BUFFER_HPP_
