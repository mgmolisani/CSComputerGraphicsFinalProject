#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

in DomainData {
  vec3 normal;
} domain_data[];

out GeometryData {
	vec4 color;
} geometry_data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float amplitude;
uniform float frequency;
uniform float phase;
uniform float grass_scale;

const vec4 dark_green = vec4(0.0f, 0.390625f, 0.0f, 1.0f);
const vec4 light_green = vec4(0.5f, 1.0f, 0.0f, 1.0f);
const vec3 grass_vertices[3] = vec3[3](
	vec3(0.5f, 0.0f, 0.0f),
	vec3(-0.5f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f)
);
const vec4 grass_colors[3] = vec4[3](
	dark_green,
	dark_green,
	light_green
);

float Rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

mat4 Rotate(float angle, vec3 axis) {
	float c = cos(angle);
	float s = sin(angle);
	float c_diff = 1.0f - c;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	return mat4(
		c + x * x * c_diff, y * x * c_diff + z * s, z * x * c_diff - y * s, 0.0f,
		x * y * c_diff - z * s, c + y * y * c_diff, z * y * c_diff + x * s, 0.0f,
		x * z * c_diff + y * s, y * z * c_diff - x * s, c + z * z * c_diff, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void main() {
	vec4 root_position = model * gl_in[0].gl_Position;

	mat4 offset = mat4(1.0f);
	offset[3] = root_position;

	mat4 scale = mat4(1.0f);
	scale[0][0] = grass_scale * 0.1f;
	scale[1][1] = grass_scale * 1.25f;

	float random = Rand(root_position.xy);
	mat4 rotation_y = Rotate(radians(random * 360.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 rotation_x = Rotate(radians(random * 30.0f), vec3(1.0f, 0.0f, 0.0f));

	/*
	We don't have any tangents,
	but for this, any coordinates where the normal is the up direction of the grass will do.
	It just makes the rotation around the up direction unecessary for irregular geometries.
	To do this, we convert to tangent space by rotating using the up and normal dot and cross products
	*/
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	vec3 normal_component = normalize(normal_matrix * domain_data[0].normal);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	mat4 tangent_matrix = Rotate(
		acos(dot(up, normal_component)),
		cross(up, normal_component) // already normalized
	);

	/*
	Rotation from wind should be around the axis perpendicular to the normal of the grass in tangent space and the wind direction.
	It also should move somewhat proportionally to the angle it makes with the wind direction.
	(i.e. blades opposing the wind move more than ones already following the wind stream)
	*/
	vec3 wind_direction = vec3(-1.0f, 0.0f, 0.0f);
	mat4 wind = Rotate(
		radians((amplitude * sin(frequency * length(root_position.xyz) + phase) + amplitude) * 15.0f * (1.0f - abs(dot(wind_direction, normal_component)))),
		cross(normal_component, wind_direction)
	);

	// // For comparison to arbitrary tangent space on flat plane
	// vec3 tangent_component = normalize(normal_matrix * domain_data[0].tangent);
	// tangent_component = normalize(
	// 	tangent_component - dot(tangent_component, normal_component) * normal_component
	// );
	// vec3 bitangent_component = cross(normal_component, tangent_component);
	// mat3 bnt_matrix = mat3(bitangent_component, normal_component, tangent_component);
	// //tangent_matrix = mat4(bnt_matrix);

	for (int i = 0; i < 3; i++) {
		vec4 position = vec4(grass_vertices[i], 1.0f);

		position = scale * position;
		position = rotation_x * position;
		position = rotation_y * position;
		position = tangent_matrix * position;
		position = wind * position;
		position = offset * position;

		gl_Position = projection * view * position;
		geometry_data.color = grass_colors[i];
		EmitVertex();
	}

  EndPrimitive();
} 
