#version 410 core
layout(triangles, equal_spacing, cw, point_mode) in;

in VertexData {
  vec3 normal;
}  vertex_data[];

out DomainData {
  vec3 normal;
} domain_data;

vec3 interpolate(vec3 v0, vec3 v1, vec3 v2) {
  return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

void main() {
	domain_data.normal = interpolate(
		vertex_data[0].normal,
		vertex_data[1].normal,
		vertex_data[2].normal
	);

	gl_Position = vec4(
		interpolate(
			vec3(gl_in[0].gl_Position),
			vec3(gl_in[1].gl_Position),
			vec3(gl_in[2].gl_Position)
		),
		1.0f
	);
}