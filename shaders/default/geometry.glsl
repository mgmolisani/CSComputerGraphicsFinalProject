#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	for (int i = 0; i < 3; i++) {
		gl_Position = projection * view * model * gl_in[i].gl_Position;
		EmitVertex();
	}

  EndPrimitive();
} 
