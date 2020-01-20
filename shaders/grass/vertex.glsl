#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VertexData {
  vec3 normal;
} vertex_data;

void main() {
	vertex_data.normal = normal;
  gl_Position = vec4(position, 1.0f);
}