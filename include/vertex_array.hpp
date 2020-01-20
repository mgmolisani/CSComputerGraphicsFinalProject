#ifndef CS5310_VERTEX_ARRAY_HPP_
#define CS5310_VERTEX_ARRAY_HPP_

#include "glad/glad.h"

class VertexArray {
 public:
  VertexArray();
  ~VertexArray();

	void Bind() const;

  static void AttribPointer(const GLuint index, const GLint size,
                            const GLenum type, const GLboolean normalized,
                            const GLsizei stride, const GLvoid *const pointer);
  static void EnableAttribArray(const GLuint index);
  static void DisableAttribArray(const GLuint index);

 private:
  GLuint id_;
};

#endif  // CS5310_VERTEX_ARRAY_HPP_
