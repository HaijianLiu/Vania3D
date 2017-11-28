
#version 330 core
layout (location = 0) in vec3 positionVertex;
layout (location = 1) in vec2 uvVertex;
layout (location = 2) in vec3 normalVertex;

out vec3 position;
out vec2 uv;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

void main() {
	position = vec3(model * vec4(positionVertex, 1.0));
	normal = vec3(model * vec4(normalVertex, 0.0));
	uv = uvVertex;

	float wave = 0.01 * sin(time);

	gl_Position =  projection * view * model * vec4(positionVertex.x, positionVertex.y, positionVertex.z + wave, 1.0);
}
