#version 330 core
layout (location = 0) out vec4 albedoColor;
layout (location = 1) out vec4 normalColor;
layout (location = 2) out vec4 mraColor;
layout (location = 3) out vec4 worldPos;
layout (location = 4) out vec4 alpha;

// out vec4 FragColor;
// in vec2 TexCoords;
in vec3 WorldPos;
// in vec3 Normal;

void main() {
	albedoColor = vec4(1.0);
	normalColor = vec4(0.0, 0.0, 1.0, 1.0);
	mraColor = vec4(1.0);
	worldPos = vec4(WorldPos, 1.0);
	alpha = vec4(1.0);
}
