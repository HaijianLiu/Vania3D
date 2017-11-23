
#version 330 core
layout (location = 0) in vec3 positionVertex;
layout (location = 1) in vec2 uvVertex;
layout (location = 2) in vec3 lightColor;
layout (location = 3) in vec3 lightPosition;

out vec2 uv;

void main() {
	uv = uvVertex;
	gl_Position = vec4(positionVertex, 1.0);
}
