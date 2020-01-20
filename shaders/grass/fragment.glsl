#version 410 core
in GeometryData {
	vec4 color;
} geometry_data;

out vec4 fragment_color;

void main() {
  fragment_color = geometry_data.color;
} 